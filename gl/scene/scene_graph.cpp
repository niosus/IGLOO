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
  CHECK_GT(storage_.count(parent_key), 0u) << "Must have a real parent";
  storage_.emplace(
      new_key,
      std::make_unique<Node>(
          new_key, parent_key, drawable, tx_parent_local, &storage_));
  storage_.at(parent_key)->AddChildKey(new_key);
  return new_key;
}

int SceneGraph::Erase(Key key) {
  std::lock_guard<decltype(graph_mutex)> guard(graph_mutex);
  if (!storage_.count(key)) { return 0; }
  int erased_counter = EraseChildren(key);

  // Erase itself from parent if it has one.
  const auto& node_ptr = storage_.at(key);
  if (storage_.count(node_ptr->parent_key()) > 0) {
    CHECK(storage_.at(node_ptr->parent_key()));
    storage_.at(node_ptr->parent_key())->EraseChildKey(key);
  }

  // Erase itself from storage.
  erased_counter += storage_.erase(key);
  return erased_counter;
}

int SceneGraph::EraseChildren(Key key) {
  CHECK_GT(storage_.count(key), 0u);
  // Erase children.
  const auto& node_ptr = storage_.at(key);
  int total_erased_count = 0;
  // Need a copy here, as Erase invalidates the iterators, thus causing an
  // undefined behavior if we do not create this copy.
  const auto children_keys{node_ptr->children_keys()};
  for (const auto& child_key : children_keys) {
    total_erased_count += Erase(child_key);
  }
  return total_erased_count;
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

}  // namespace gl
