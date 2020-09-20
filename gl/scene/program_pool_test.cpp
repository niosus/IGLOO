// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/program_pool.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(ProgramPoolTest, Simple) {
  auto& pool{ProgramPool::Instance()};
  const auto& program{pool.AddProgramFromShaders(
      ProgramPool::ProgramType::DRAW_POINTS,
      {"gl/scene/shaders/points.vert", "gl/scene/shaders/simple.frag"})};
  ASSERT_NE(program.get(), nullptr);
  const auto& same_program{
      pool.GetProgram(ProgramPool::ProgramType::DRAW_POINTS)};
  ASSERT_EQ(program.get(), same_program.get());
  pool.Clear();
}
