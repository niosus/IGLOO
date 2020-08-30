// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#include "gl/scene/scene_graph.h"

#include <Eigen/Geometry>

namespace gl {

SceneGraph::Key SceneGraph::global_node_counter_ = 0;
const SceneGraph::Key SceneGraph::kRootKey = SceneGraph::GenerateNextKey();

SceneGraph::Key SceneGraph::GenerateNextKey() { return global_node_counter_++; }

SceneGraph::SceneGraph() {}

SceneGraph::Node& SceneGraph::GetNode(Key key) {
  CHECK_GT(storage_.count(key), 0u);
  return *storage_.at(key);
}
const SceneGraph::Node& SceneGraph::GetNode(Key key) const {
  CHECK_GT(storage_.count(key), 0u);
  return *storage_.at(key);
}

SceneGraph::Key SceneGraph::RegisterBranchKey(Key key) {
  std::lock_guard<std::recursive_mutex> guard(graph_mutex);
  if (storage_.count(key) > 0) {
    LOG(WARNING) << "Key " << key << " already registered in scene graph.";
    return false;
  }
  // Our root is a null drawable that has base key as the parent.
  auto branch_root = std::make_unique<Node>(
      key, kRootKey, nullptr, Eigen::Isometry3f::Identity(), &storage_);

  // Add it to storage.
  storage_.emplace(key, std::move(branch_root));
  return key;
}

SceneGraph::Key SceneGraph::Attach(Key parent_key,
                                   Drawable::SharedPtr drawable,
                                   const Eigen::Isometry3f& tx_parent_local,
                                   Key new_key) {
  std::lock_guard<decltype(graph_mutex)> guard(graph_mutex);
  CHECK_GT(storage_.count(parent_key), 0u) << "New node must have a parent";
  storage_.emplace(
      new_key,
      std::make_unique<Node>(
          new_key, parent_key, drawable, tx_parent_local, &storage_));
  storage_.at(parent_key)->AddChildKey(new_key);
  return new_key;
}

int SceneGraph::Erase(Key key) { return NodeEraser{&storage_}.Erase(key); }

int SceneGraph::EraseChildren(Key key) {
  return NodeEraser{&storage_}.EraseChildren(key);
}

void SceneGraph::Draw(Key key) {
  std::lock_guard<decltype(graph_mutex)> guard(graph_mutex);
  CHECK_GT(storage_.count(key), 0u);
  GetNode(key).Draw();
}

SceneGraph::Node::Node(SceneGraph::Key node_key,
                       SceneGraph::Key parent_key,
                       Drawable::SharedPtr drawable,
                       const Eigen::Isometry3f& tx_parent_local,
                       SceneGraph::Storage<UniquePtr>* storage)
    : key_{node_key},
      parent_key_{parent_key},
      tx_parent_local_{tx_parent_local},
      drawable_{drawable},
      storage_{storage} {
  CHECK_NOTNULL(storage_);
}

void SceneGraph::Node::Draw(const Eigen::Isometry3f& tx_accumulated) const {
  CHECK_NOTNULL(storage_);
  Eigen::Isometry3f tx_world_local = tx_accumulated * tx_parent_local_;
  if (drawable_) {
    drawable_->SetModel(tx_world_local.matrix());
    if (!drawable_->ready_to_draw()) { drawable_->FillBuffers(); }
    drawable_->Draw();
  }
  for (const auto& child_key : children_keys_) {
    DCHECK_GT(storage_->count(child_key), 0u);
    const auto& child = storage_->at(child_key);
    child->Draw(tx_world_local);
  }
}

Eigen::Isometry3f SceneGraph::Node::ComputeTxAccumulated() const {
  CHECK_NOTNULL(storage_);
  Eigen::Isometry3f tx_accumulated = tx_parent_local_;
  Key parent_key = parent_key_;
  while (parent_key != kRootKey) {
    DCHECK_GT(storage_->count(parent_key), static_cast<size_t>(0));
    auto& parent_node = storage_->at(parent_key);
    tx_accumulated = parent_node->tx_parent_local() * tx_accumulated;
    parent_key = parent_node->parent_key();
  }
  return tx_accumulated;
}

SceneGraph::NodeEraser::NodeEraser(
    SceneGraph::Storage<Node::UniquePtr>* storage)
    : storage_{storage} {}

int SceneGraph::NodeEraser::EraseChildren(Key key) {
  CHECK_NOTNULL(storage_);
  CHECK_GT(storage_->count(key), 0u);
  std::set<Key> children{};
  FindRecursiveChildren(key, &children);
  for (auto child_key : children) { storage_->erase(child_key); }
  auto& node{storage_->at(key)};
  node->ClearChildKeys();
  return static_cast<int>(children.size());
}

int SceneGraph::NodeEraser::Erase(Key key) {
  CHECK_NOTNULL(storage_);
  if (storage_->count(key) < 1) { return 0; }
  const auto erased_nodes_count{EraseChildren(key)};
  const auto& node{storage_->at(key)};
  auto& parent_node{storage_->at(node->parent_key())};
  parent_node->EraseChildKey(key);
  storage_->erase(key);
  return erased_nodes_count + 1;
}

void SceneGraph::NodeEraser::FindRecursiveChildren(Key key,
                                                   std::set<Key>* children) {
  const auto& node{storage_->at(key)};
  const auto& current_children_keys{node->children_keys()};
  for (auto key : current_children_keys) {
    children->insert(key);
    FindRecursiveChildren(key, children);
  }
}

}  // namespace gl
