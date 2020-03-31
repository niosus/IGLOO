// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#pragma once
#include <ipb_opengl_tools/gl/drawables/drawable.h>

#include <Eigen/Geometry>

#include <map>
#include <mutex>
#include <set>
#include <unordered_map>

namespace ipb {
namespace vis {

/// A scene graph class where all drawables are organized into a tree where the
/// edges hold the relative transformation information.
class SceneGraph {
  class Node;  // Forward declare.

 public:
  using Key = uint64_t;

  template <class T>
  using Storage = std::unordered_map<Key, T>;

  template <class T>
  struct NodeData {
    Key key;
    T data;
  };

  SceneGraph();

  /// Register a key that will be a root of a branch. Example: world, odometry,
  /// etc. These also define a style they should be drawn in, e.g. 2D, 3D.
  Key RegisterBranchKey(Key key);

  /// Attach a new drawable to a parent with a relative transformation from
  /// child to parent.
  Key Attach(
      Key parent_key,
      Drawable::SharedPtr drawable,
      const Eigen::Isometry3f& tx_parent_local = Eigen::Isometry3f::Identity(),
      Key new_key = SceneGraph::GenerateNextKey());

  /// Get a node reference at the key for changing it.
  SceneGraph::Node& GetNode(Key key);
  /// Get a const node reference at the key.
  const SceneGraph::Node& GetNode(Key key) const;

  /// Draw a key with all its children.
  void Draw(Key key);

  /// Erase the node and all its children.
  int Erase(Key key);
  /// Erase all children of the node, but leave the node intact.
  int EraseChildren(Key key);

  inline size_t size() const { return storage_.size(); }
  inline bool HasNode(Key key) const { return storage_.count(key) > 0; }

  /// Generate the next key to draw.
  static Key GenerateNextKey();

 private:
  /// A single node of the graph. Holds a pointer to storage of all nodes.
  class Node {
   public:
    using UniquePtr = std::unique_ptr<Node>;
    using SharedPtr = std::shared_ptr<Node>;

    Node(Key node_key,
         Key parent_key,
         Drawable::SharedPtr drawable,
         const Eigen::Isometry3f& tx_parent_local_,
         SceneGraph::Storage<UniquePtr>* storage);

    /// Add a child key to the node.
    inline void AddChildKey(Key key) { children_keys_.emplace(key); }
    /// Erase a child key from the node.
    inline int EraseChildKey(Key key) { return children_keys_.erase(key); }

    /// Draw this node at the correct position in the world.
    void Draw(const Eigen::Isometry3f& tx_accumulated =
                  Eigen::Isometry3f::Identity()) const;
    Eigen::Isometry3f ComputeTxAccumulated() const;

    Key key() const { return key_; }
    Key parent_key() const { return parent_key_; }

    Eigen::Isometry3f& tx_parent_local() { return tx_parent_local_; }
    const Eigen::Isometry3f& tx_parent_local() const {
      return tx_parent_local_;
    }

    const std::set<Key>& children_keys() const { return children_keys_; }

    const Drawable::SharedPtr& drawable() const { return drawable_; }

   private:
    /// Key of this node.
    Key key_;
    /// A key of a direct parent of this node.
    Key parent_key_;
    /// Keys of all children of this node.
    std::set<Key> children_keys_;

    /// Relative transformation from this coordinate frame to parent's one.
    Eigen::Isometry3f tx_parent_local_;

    /// Every node is storing a drawable.
    Drawable::SharedPtr drawable_ = nullptr;

    /// They nodes are stored elsewhere. We need the pointer to this place.
    SceneGraph::Storage<Node::UniquePtr>* storage_ = nullptr;
  };

  static const Key kRootKey;
  static Key global_node_counter_;

  Storage<Node::UniquePtr> storage_;

  std::recursive_mutex graph_mutex;
};

}  // namespace vis
}  // namespace ipb
