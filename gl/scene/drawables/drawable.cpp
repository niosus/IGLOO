// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/drawables/drawable.h"

#include <Eigen/Core>

#include <memory>
#include <vector>

ABSL_FLAG(float, drawable_point_size, 3.0f, "Default size of the points.");

namespace gl {

Drawable::Drawable(ProgramPool* program_pool,
                   ProgramPool::ProgramIndex program_index,
                   GLenum mode,
                   float point_size,
                   const Eigen::Vector3f& color)
    : program_pool_{program_pool},
      program_index_{program_index},
      mode_{mode},
      point_size_{point_size},
      color_{color} {}

void Drawable::Draw() {
  CHECK(vao_) << "Need a VAO to draw.";
  CHECK(program_pool_) << "Need a program pool to draw.";
  CHECK(program_index_) << "Need a program index to draw.";

  program_pool_->UseProgram(program_index_.value());
  if (texture_) { texture_->Bind(); }

  vao_->Draw(mode_);

  // Set the line and point sizes.
  glPointSize(point_size_);
  glLineWidth(point_size_);

  if (texture_) { texture_->UnBind(); }
}

void Drawable::ChangeColor(const Eigen::Vector3f& color) noexcept {
  color_ = color;
  if (color_uniform_index_) {
    program_pool_->UpdateUniformInActiveProgram(color_uniform_index_, color_);
  }
}

}  // namespace gl
