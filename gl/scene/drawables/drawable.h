// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef CODE_OPENGL_TUTORIALS_GL_SCENE_DRAWABLES_DRAWABLE_H_
#define CODE_OPENGL_TUTORIALS_GL_SCENE_DRAWABLES_DRAWABLE_H_

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "gl/core/program.h"
#include "gl/core/texture.h"
#include "gl/core/vertex_array_buffer.h"
#include "gl/scene/program_pool.h"
#include "gl/utils/eigen_traits.h"
#include "glog/logging.h"

#include <Eigen/Core>

#include <memory>
#include <vector>

ABSL_DECLARE_FLAG(float, drawable_point_size);

namespace gl {

/// A generic drawable that knows how to draw itself in OpenGL context.
class Drawable {
 public:
  using Ptr = std::unique_ptr<Drawable>;
  using SharedPtr = std::shared_ptr<Drawable>;

  enum class Style { DRAW_2D, DRAW_3D };

  explicit Drawable(ProgramPool* program_pool,
                    ProgramPool::ProgramIndex program_index,
                    GLenum mode = GL_NONE,
                    float point_size = FLAGS_drawable_point_size.Get(),
                    const Eigen::Vector3f& color = Eigen::Vector3f::Ones());

  // Disable value semantics as only pointers of Drawable should be used.
  Drawable(const Drawable&) = delete;
  Drawable(Drawable&&) = delete;
  Drawable& operator=(const Drawable&) = delete;
  Drawable& operator=(Drawable&&) = delete;
  virtual ~Drawable() {}

  /// A funtion that must be called in order to allow drawing a drawable.
  /// It is needed to make sure we always allocate gpu memory from same thread.
  virtual void FillBuffers() {}

  /// Check if the drawable has buffers filled.
  inline bool ready_to_draw() const { return ready_to_draw_; }

  /// A function that is responsible for binding all the buffers and actually
  /// drawing this drawable.
  void Draw();

  /// Change the color of this drawable if it supports color.
  void ChangeColor(const Eigen::Vector3f& color) noexcept;

  // TODO(igor): this somehow smells bad. Do I need this function? What do I
  // need it for?
  inline void SetModel(const Eigen::Matrix4f& model) const {
    CHECK(program_index_);
    CHECK_GT(model_uniform_index_, 0UL);
    program_pool_->UseProgram(program_index_.value());
    program_pool_->UpdateUniformInActiveProgram(model_uniform_index_, model);
  }

 protected:
  // A program pool to be used when filling buffers or drawing.
  ProgramPool* program_pool_{nullptr};
  /// Every drawable shares ownership over the OpenGL program it uses.
  std::optional<ProgramPool::ProgramIndex> program_index_{};

  /// Model matrix that defines where this drawable is situated in the world.
  std::size_t model_uniform_index_{};
  /// A uniform for tweaking the projection-view matrix.
  std::size_t projection_view_uniform_index_{};
  /// A uniform to set color to the points.
  std::size_t color_uniform_index_{};

  /// This maps to the OpenGL modes, e.g. GL_TRIANGLES.
  GLenum mode_{GL_NONE};

  /// A drawable can share a texture that it draws.
  std::shared_ptr<Texture> texture_{nullptr};
  /// A drawable owns a vertex array object.
  std::unique_ptr<VertexArrayBuffer> vao_{nullptr};

  /// This is false by default or if some change was made to the drawable. This
  /// is used to trigger when we want to fill the buffers.
  bool ready_to_draw_{false};

  /// Size of points and lines used when drawing.
  float point_size_{};
  /// Color of this drawable.
  Eigen::Vector3f color_{Eigen::Vector3f::Ones()};
};

}  // namespace gl

#endif  // CODE_OPENGL_TUTORIALS_GL_SCENE_DRAWABLES_DRAWABLE_H_
