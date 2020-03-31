// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#pragma once

#include <glow/GlBuffer.h>
#include <glow/GlTexture.h>
#include <glow/GlUniform.h>
#include <glow/GlVertexArray.h>
#include <glow/glutil.h>

#include <gflags/gflags.h>

#include <ipb_opengl_tools/gl/program.h>

#include <Eigen/Core>

#include <memory>
#include <vector>

DECLARE_int32(drawable_point_size);

namespace ipb {
namespace vis {

/// A generic drawable that knows how to draw itself in OpenGL context.
class Drawable {
 public:
  using Ptr = std::unique_ptr<Drawable>;
  using SharedPtr = std::shared_ptr<Drawable>;

  enum class Style { DRAW_2D, DRAW_3D };

  explicit Drawable(Style style,
                    GLenum mode = GL_NONE,
                    float point_size = FLAGS_drawable_point_size,
                    const glow::vec3& color = {1.0f, 1.0f, 1.0f});
  virtual ~Drawable() {}

  /// A funtion that must be called in order to allow drawing a drawable.
  /// It is needed to make sure we always allocate gpu memory from same thread.
  virtual void FillBuffers() {}

  /// Check if the drawable has buffers filled.
  inline bool ready_to_draw() const { return ready_to_draw_; }

  /// A function that is responsible for binding all the buffers and actually
  /// drawing this drawable.
  void Draw();

  inline const glow::GlUniform<Eigen::Matrix4f>& model() const {
    return model_;
  }
  inline glow::GlUniform<Eigen::Matrix4f>& model() { return model_; }

  inline Style draw_style() const { return draw_style_; }

 protected:
  using UniformPtr = std::unique_ptr<glow::GlAbstractUniform>;
  using IndexBufferT = glow::GlBuffer<GLushort>;

  /// Every drawable shares ownership over the OpenGL program it uses.
  Program::SharedPtr program_ = nullptr;

  size_t num_points_ = 0;

  /// Which is the draw style of this drawable.
  Style draw_style_;

  /// This maps to the OpenGL modes, e.g. GL_TRIANGLES.
  GLenum mode_ = GL_NONE;

  /// A drawable owns a texture that it draws.
  std::unique_ptr<glow::GlTexture> texture_ = nullptr;
  /// A drawable owns a vertex array object.
  std::unique_ptr<glow::GlVertexArray> vao_ = nullptr;
  /// A drawable owns an index buffer.
  std::unique_ptr<IndexBufferT> indices_buffer_ = nullptr;

  /// Model matrix that defines where this drawable is situated in the world.
  glow::GlUniform<Eigen::Matrix4f> model_ = glow::GlUniform<Eigen::Matrix4f>(
      Program::kModelUniform, Eigen::Matrix4f::Identity());

  /// All uniforms that are defined by a drawable that must be **overridden**
  /// before every draw. Only store here those that need to be applied on every
  /// draw. This can be used for example to set the projection matrix to
  /// identity to draw 2d things.
  std::vector<UniformPtr> uniforms_to_override_;

  int sampler_index_ = -1;

  /// This is false by default or if some change was made to the drawable. This
  /// is used to trigger when we want to fill the buffers.
  bool ready_to_draw_ = false;

  /// Size of points and lines used when drawing.
  float point_size_;
  /// Color of this drawable.
  glow::vec3 color_;
};

}  // namespace vis
}  // namespace ipb
