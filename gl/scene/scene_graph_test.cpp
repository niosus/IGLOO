// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include <gtest/gtest.h>

#include "gl/scene/scene_graph.h"

using gl::Drawable;
using gl::SceneGraph;

SceneGraph::Key world_key = SceneGraph::GenerateNextKey();

TEST(SceneGraphTest, Init) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_TRUE(graph.HasNode(world_key));
}

TEST(SceneGraphTest, ForgetInit) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  SceneGraph graph;
  EXPECT_DEATH(
      graph.SceneGraph::Attach(
          world_key, std::make_shared<Drawable>(Drawable::Style::DRAW_3D)),
      "Must have a real parent");
}

TEST(SceneGraphTest, StoringDrawable) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  auto drawable = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto key = graph.Attach(world_key, drawable);
  EXPECT_EQ(graph.GetNode(key).drawable().get(), drawable.get());
}

TEST(SceneGraphTest, StoringMoreDrawables) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);  // This has a world drawable that points to base.
  auto drawable_1 = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto drawable_2 = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto key_1 = graph.Attach(world_key, drawable_1);
  auto key_2 = graph.Attach(world_key, drawable_2);
  EXPECT_EQ(graph.size(), 3u);
  EXPECT_NE(graph.GetNode(key_1).drawable().get(),
            graph.GetNode(key_2).drawable().get());
}

TEST(SceneGraphTest, Transform) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  auto drawable_1 = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto drawable_2 = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto key_1 = graph.Attach(world_key, drawable_1);
  Eigen::Isometry3f test_transform = Eigen::Isometry3f::Identity();
  test_transform.translation() = Eigen::Vector3f{1, 1, 1};
  auto key_2 = graph.Attach(world_key, drawable_2, test_transform);
  EXPECT_EQ(graph.size(), 3u);
  EXPECT_EQ(graph.GetNode(key_1).tx_parent_local().translation().x(), 0);
  EXPECT_EQ(graph.GetNode(key_1).tx_parent_local().translation().y(), 0);
  EXPECT_EQ(graph.GetNode(key_1).tx_parent_local().translation().z(), 0);
  EXPECT_EQ(graph.GetNode(key_2).tx_parent_local().translation().x(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tx_parent_local().translation().y(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tx_parent_local().translation().z(), 1);
}

TEST(SceneGraphTest, ChainTransform) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  auto drawable = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  Eigen::Isometry3f test_transform = Eigen::Isometry3f::Identity();
  test_transform.translation() = Eigen::Vector3f{1, 1, 1};
  auto key_1 = graph.Attach(world_key, drawable, test_transform);
  auto key_2 = graph.Attach(key_1, drawable, test_transform);
  EXPECT_EQ(graph.size(), 3u);
  EXPECT_EQ(graph.GetNode(key_1).tx_parent_local().translation().x(), 1);
  EXPECT_EQ(graph.GetNode(key_1).tx_parent_local().translation().y(), 1);
  EXPECT_EQ(graph.GetNode(key_1).tx_parent_local().translation().z(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tx_parent_local().translation().x(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tx_parent_local().translation().y(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tx_parent_local().translation().z(), 1);
  EXPECT_EQ(graph.GetNode(key_2).ComputeTxAccumulated().translation().x(), 2);
  EXPECT_EQ(graph.GetNode(key_2).ComputeTxAccumulated().translation().y(), 2);
  EXPECT_EQ(graph.GetNode(key_2).ComputeTxAccumulated().translation().z(), 2);
}

TEST(SceneGraphTest, SimpleErase) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);
  auto drawable_1 = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto key = graph.Attach(world_key, drawable_1);
  EXPECT_EQ(graph.size(), 2u);
  graph.Erase(key);
  EXPECT_EQ(graph.size(), 1u);
}

TEST(SceneGraphTest, SimpleEraseChildren) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);
  auto drawable = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto key_1 = graph.Attach(world_key, drawable);
  auto key_2 = graph.Attach(world_key, drawable);
  auto key_3 = graph.Attach(key_1, drawable);
  auto key_4 = graph.Attach(key_1, drawable);
  auto key_5 = graph.Attach(key_3, drawable);
  EXPECT_EQ(graph.size(), 6u);
  EXPECT_TRUE(graph.HasNode(key_1));
  EXPECT_TRUE(graph.HasNode(key_2));
  EXPECT_TRUE(graph.HasNode(key_3));
  EXPECT_TRUE(graph.HasNode(key_4));
  EXPECT_TRUE(graph.HasNode(key_5));
  EXPECT_EQ(3, graph.EraseChildren(key_1));
  EXPECT_EQ(graph.size(), 3u);
  EXPECT_TRUE(graph.HasNode(key_1));
  EXPECT_TRUE(graph.HasNode(key_2));

  EXPECT_FALSE(graph.HasNode(key_3));
  EXPECT_FALSE(graph.HasNode(key_4));
  EXPECT_FALSE(graph.HasNode(key_5));
}

TEST(SceneGraphTest, HasNode) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  auto drawable = std::make_shared<Drawable>(Drawable::Style::DRAW_3D);
  auto key = graph.Attach(world_key, drawable);
  EXPECT_TRUE(graph.HasNode(key));
  graph.Erase(key);
  EXPECT_FALSE(graph.HasNode(key));
}

TEST(SceneGraphTest, ChainedErase) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);
  auto key_1 = graph.Attach(
      world_key, std::make_shared<Drawable>(Drawable::Style::DRAW_3D));
  auto key_2 =
      graph.Attach(key_1, std::make_shared<Drawable>(Drawable::Style::DRAW_3D));
  auto key_3 =
      graph.Attach(key_2, std::make_shared<Drawable>(Drawable::Style::DRAW_3D));
  EXPECT_TRUE(graph.HasNode(key_1));
  EXPECT_TRUE(graph.HasNode(key_2));
  EXPECT_TRUE(graph.HasNode(key_3));
  graph.Erase(key_2);
  EXPECT_FALSE(graph.HasNode(key_2));
  EXPECT_FALSE(graph.HasNode(key_3));
}

TEST(SceneGraphTest, BroadErase) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1u);
  auto key_1 = graph.Attach(
      world_key, std::make_shared<Drawable>(Drawable::Style::DRAW_3D));
  auto key_2 =
      graph.Attach(key_1, std::make_shared<Drawable>(Drawable::Style::DRAW_3D));
  auto key_3 =
      graph.Attach(key_1, std::make_shared<Drawable>(Drawable::Style::DRAW_3D));
  auto key_4 =
      graph.Attach(key_2, std::make_shared<Drawable>(Drawable::Style::DRAW_3D));
  EXPECT_TRUE(graph.HasNode(key_1));
  EXPECT_TRUE(graph.HasNode(key_2));
  EXPECT_TRUE(graph.HasNode(key_3));
  EXPECT_TRUE(graph.HasNode(key_4));
  graph.Erase(key_1);
  EXPECT_FALSE(graph.HasNode(key_1));
  EXPECT_FALSE(graph.HasNode(key_2));
  EXPECT_FALSE(graph.HasNode(key_3));
  EXPECT_FALSE(graph.HasNode(key_4));
}
