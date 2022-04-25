// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/program_pool.h"
#include "gtest/gtest.h"

using gl::ProgramPool;
using gl::Shader;

TEST(ProgramPoolTest, Simple) {
  ProgramPool pool{};
  const auto points_program_index{
      pool.AddProgramFromShaders(Shader::CreateFromFiles(
          {"gl/scene/shaders/points.vert", "gl/scene/shaders/simple.frag"}))};
  ASSERT_TRUE(points_program_index.has_value());
  ASSERT_EQ(points_program_index.value(), 0UL);
  const auto another_points_program_index{
      pool.AddProgramFromShaders(Shader::CreateFromFiles(
          {"gl/scene/shaders/points.vert", "gl/scene/shaders/simple.frag"}))};
  ASSERT_TRUE(another_points_program_index.has_value());
  ASSERT_EQ(another_points_program_index.value(), 1UL);
}

// TODO(igor): add more tests here
