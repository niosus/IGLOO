// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#include <ipb_opengl_tools/gl/drawables/drawable.h>

#include <glow/GlUniform.h>
#include <glow/GlVertexArray.h>
#include <glow/ScopedBinder.h>

#include <ipb_opengl_tools/gl/program.h>

#include <Eigen/Core>

#include <memory>
#include <vector>

DEFINE_int32(drawable_point_size, 3, "Default size of the points.");

namespace ipb {
namespace vis {

Drawable::Drawable(Style style,
                   GLenum mode,
                   float point_size,
                   const glow::vec3& color)
    : draw_style_{style}, mode_{mode}, point_size_{point_size}, color_{color} {
  if (draw_style_ == Style::DRAW_2D) {
    uniforms_to_override_.emplace_back(
        ipb::make_unique<glow::GlUniform<Eigen::Matrix4f>>(
            Program::kProjViewUniform, Eigen::Matrix4f::Identity()));
  }
}

void Drawable::Draw() {
  CHECK(vao_) << "Need a VAO to draw.";
  CHECK(program_) << "Need a Program to draw.";
  CHECK_GT(num_points_, static_cast<size_t>(0))
      << "Need the number of points to draw.";

  if (texture_) {
    CHECK_GE(sampler_index_, 0) << "Sampler must be non-negative";
    glActiveTexture(GL_TEXTURE0 + sampler_index_);
    texture_->bind();
  }

  auto vao_guard = glow::ScopedBinder<glow::GlVertexArray>(*vao_);
  auto program_guard = glow::ScopedBinder<glow::GlProgram>(*program_);

  program_->setUniform(model_);
  program_->SetAllUniforms(uniforms_to_override_);
  program_->setUniform(glow::GlUniform<glow::vec3>("color", color_));

  // Set the line and point sizes.
  glPointSize(point_size_);
  glLineWidth(point_size_);

  if (indices_buffer_) {
    glDrawElements(mode_, num_points_, GL_UNSIGNED_SHORT, nullptr);
  } else {
    glDrawArrays(mode_, 0, num_points_);
  }

  if (texture_) { texture_->release(); }
}

}  // namespace vis
}  // namespace ipb
