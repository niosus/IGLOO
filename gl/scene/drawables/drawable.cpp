// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/drawables/drawable.h"

#include <Eigen/Core>

#include <memory>
#include <vector>

ABSL_FLAG(float, drawable_point_size, 3.0f, "Default size of the points.");

namespace gl {

Drawable::Drawable(const ProgramPool& program_pool,
                   const ProgramPool::ProgramType& program_type,
                   Style style,
                   GLenum mode,
                   float point_size,
                   const Eigen::Vector3f& color)
    : program_{program_pool.GetProgram(program_type)},
      draw_style_{style},
      mode_{mode},
      point_size_{point_size},
      color_{color} {}

void Drawable::Draw() {
  CHECK(vao_) << "Need a VAO to draw.";
  CHECK(program_) << "Need a Program to draw.";

  program_->Use();
  if (color_uniform_index_) {
    program_->GetUniform(color_uniform_index_).UpdateValue(color_);
  }

  if (texture_) { texture_->Bind(); }

  vao_->Draw(mode_);

  // Set the line and point sizes.
  glPointSize(point_size_);
  glLineWidth(point_size_);

  if (texture_) { texture_->UnBind(); }
}

}  // namespace gl
