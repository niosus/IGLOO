// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include <gtest/gtest.h>

#include "gl/scene/scene_graph.h"

using gl::Drawable;
using gl::ProgramPool;
using gl::SceneGraph;

class SceneGraphTest : public ::testing::Test {
 protected:
  void SetUp() override {
    program_pool_.AddProgramFromShaders(
        ProgramPool::ProgramType::DRAW_POINTS,
        {"gl/scene/shaders/points.vert", "gl/scene/shaders/simple.frag"});
    default_drawable_ =
        std::make_shared<Drawable>(program_pool_,
                                   ProgramPool::ProgramType::DRAW_POINTS,
                                   Drawable::Style::DRAW_3D);
  }

  void TearDown() override { program_pool_.Clear(); }

  ProgramPool program_pool_{};
  std::shared_ptr<Drawable> default_drawable_{nullptr};
};

SceneGraph::Key world_key = SceneGraph::GenerateNextKey();

TEST_F(SceneGraphTest, Init) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_TRUE(graph.HasNode(world_key));
}

TEST_F(SceneGraphTest, ForgetInit) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  SceneGraph graph;
  EXPECT_DEATH(
      graph.SceneGraph::Attach(
          world_key,
          std::make_shared<Drawable>(program_pool_,
                                     ProgramPool::ProgramType::DRAW_POINTS,
                                     Drawable::Style::DRAW_3D)),
      "New node must have a parent");
}

TEST_F(SceneGraphTest, StoringDrawable) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  auto key = graph.Attach(world_key, default_drawable_);
  EXPECT_EQ(graph.GetNode(key).drawable().get(), default_drawable_.get());
}

TEST_F(SceneGraphTest, StoringMoreDrawables) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);  // This has a world drawable that points to base.
  auto other_drawable =
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D);
  auto key_1 = graph.Attach(world_key, default_drawable_);
  auto key_2 = graph.Attach(world_key, other_drawable);
  EXPECT_EQ(graph.size(), 3u);
  EXPECT_NE(graph.GetNode(key_1).drawable().get(),
            graph.GetNode(key_2).drawable().get());
}

TEST_F(SceneGraphTest, Transform) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  auto other_drawable =
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D);
  auto key_1 = graph.Attach(world_key, default_drawable_);
  Eigen::Isometry3f test_transform = Eigen::Isometry3f::Identity();
  test_transform.translation() = Eigen::Vector3f{1, 1, 1};
  auto key_2 = graph.Attach(world_key, other_drawable, test_transform);
  EXPECT_EQ(graph.size(), 3u);
  EXPECT_EQ(graph.GetNode(key_1).tf__parent__local().translation().x(), 0);
  EXPECT_EQ(graph.GetNode(key_1).tf__parent__local().translation().y(), 0);
  EXPECT_EQ(graph.GetNode(key_1).tf__parent__local().translation().z(), 0);
  EXPECT_EQ(graph.GetNode(key_2).tf__parent__local().translation().x(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tf__parent__local().translation().y(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tf__parent__local().translation().z(), 1);
}

TEST_F(SceneGraphTest, ChainTransform) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  Eigen::Isometry3f test_transform = Eigen::Isometry3f::Identity();
  test_transform.translation() = Eigen::Vector3f{1, 1, 1};
  auto key_1 = graph.Attach(world_key, default_drawable_, test_transform);
  auto key_2 = graph.Attach(key_1, default_drawable_, test_transform);
  EXPECT_EQ(graph.size(), 3u);
  EXPECT_EQ(graph.GetNode(key_1).tf__parent__local().translation().x(), 1);
  EXPECT_EQ(graph.GetNode(key_1).tf__parent__local().translation().y(), 1);
  EXPECT_EQ(graph.GetNode(key_1).tf__parent__local().translation().z(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tf__parent__local().translation().x(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tf__parent__local().translation().y(), 1);
  EXPECT_EQ(graph.GetNode(key_2).tf__parent__local().translation().z(), 1);
  EXPECT_EQ(graph.GetNode(key_2).ComputeTxAccumulated().translation().x(), 2);
  EXPECT_EQ(graph.GetNode(key_2).ComputeTxAccumulated().translation().y(), 2);
  EXPECT_EQ(graph.GetNode(key_2).ComputeTxAccumulated().translation().z(), 2);
}

TEST_F(SceneGraphTest, SimpleErase) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);
  auto key = graph.Attach(world_key, default_drawable_);
  EXPECT_EQ(graph.size(), 2u);
  graph.Erase(key);
  EXPECT_EQ(graph.size(), 1u);
}

TEST_F(SceneGraphTest, SimpleEraseChildren) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);
  auto key_1 = graph.Attach(world_key, default_drawable_);
  auto key_2 = graph.Attach(world_key, default_drawable_);
  auto key_3 = graph.Attach(key_1, default_drawable_);
  auto key_4 = graph.Attach(key_1, default_drawable_);
  auto key_5 = graph.Attach(key_3, default_drawable_);
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

TEST_F(SceneGraphTest, HasNode) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  auto key = graph.Attach(world_key, default_drawable_);
  EXPECT_TRUE(graph.HasNode(key));
  graph.Erase(key);
  EXPECT_FALSE(graph.HasNode(key));
}

TEST_F(SceneGraphTest, ChainedErase) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1);
  auto key_1 = graph.Attach(
      world_key,
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D));
  auto key_2 = graph.Attach(
      key_1,
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D));
  auto key_3 = graph.Attach(
      key_2,
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D));
  EXPECT_TRUE(graph.HasNode(key_1));
  EXPECT_TRUE(graph.HasNode(key_2));
  EXPECT_TRUE(graph.HasNode(key_3));
  graph.Erase(key_2);
  EXPECT_FALSE(graph.HasNode(key_2));
  EXPECT_FALSE(graph.HasNode(key_3));
}

TEST_F(SceneGraphTest, BroadErase) {
  SceneGraph graph;
  graph.RegisterBranchKey(world_key);
  EXPECT_EQ(graph.size(), 1u);
  auto key_1 = graph.Attach(
      world_key,
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D));
  auto key_2 = graph.Attach(
      key_1,
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D));
  auto key_3 = graph.Attach(
      key_1,
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D));
  auto key_4 = graph.Attach(
      key_2,
      std::make_shared<Drawable>(program_pool_,
                                 ProgramPool::ProgramType::DRAW_POINTS,
                                 Drawable::Style::DRAW_3D));
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
