// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef CODE_OPENGL_TUTORIALS_GL_SCENE_DRAWABLES_ALL_H_
#define CODE_OPENGL_TUTORIALS_GL_SCENE_DRAWABLES_ALL_H_

#include "glog/logging.h"

#include "gl/scene/drawables/drawable.h"
#include "utils/eigen_utils.h"
#include "utils/image.h"

namespace gl {

/// A class that is responsible for drawing points.
class Points : public Drawable {
 public:
  explicit Points(ProgramPool* program_pool,
                  ProgramPool::ProgramIndex program_index,
                  const eigen::vector<Eigen::Vector3f>& points,
                  const Eigen::Vector3f& color = {1.0f, 1.0f, 1.0f},
                  float point_size = 3.0f,
                  GLenum gl_mode = GL_POINTS);
  explicit Points(ProgramPool* program_pool,
                  ProgramPool::ProgramIndex program_index,
                  const eigen::vector<Eigen::Vector3f>& points,
                  const std::vector<float>& intensities,
                  const Eigen::Vector3f& color = {1.0f, 1.0f, 1.0f},
                  float point_size = 3.0f,
                  GLenum gl_mode = GL_POINTS);
  void FillBuffers() override;

 private:
  eigen::vector<Eigen::Vector3f> points_;
  std::vector<float> intensities_;
};

/// A class responsible for drawing lines.
class Lines : public Points {
 public:
  /// Draw lines between non-intersecting sequential pairs of points.
  explicit Lines(ProgramPool* program_pool,
                 ProgramPool::ProgramIndex program_index,
                 const eigen::vector<Eigen::Vector3f>& points,
                 const Eigen::Vector3f& color = {1.0f, 1.0f, 1.0f},
                 float line_width = 2.0f);
};

/// Draw a coordinate system.
class CoordinateSystem : public Drawable {
 public:
  CoordinateSystem(ProgramPool* program_pool,
                   ProgramPool::ProgramIndex program_index);
  void FillBuffers() override;
};

/// Draw a rectangle with a texture attached to it.
class RectWithTexture : public Drawable {
 public:
  RectWithTexture(ProgramPool* program_pool,
                  ProgramPool::ProgramIndex program_index,
                  std::shared_ptr<gl::Texture> texture,
                  const Eigen::Vector2f& size = {5.0f, 5.0f});
  void FillBuffers() override;

 private:
  Eigen::Vector2f size_;
};

/// Draw a rectangle with a texture attached to it.
class ScreenRectWithTexture : public Drawable {
 public:
  ScreenRectWithTexture(ProgramPool* program_pool,
                        ProgramPool::ProgramIndex program_index,
                        std::shared_ptr<gl::Texture> texture,
                        const Eigen::Vector2f& size = {1.0f, 1.0f});
  void FillBuffers() override;

 private:
  Eigen::Vector2f size_;
};

// /// Draw text.
// class Text : public Drawable {
//  public:
//   Text(const std::string& text,
//        const std::string& font_name,
//        const glow::vec3& pos,
//        float scale,
//        Style draw_style);
//   Text(const std::string& text,
//        const std::string& font_name,
//        const glow::vec3& pos,
//        float scale = 4.0f);
//   Text(const std::string& text,
//        const std::string& font_name,
//        const glow::vec2& pos,
//        float scale = 0.5f);
//   void FillBuffers() override;

//  private:
//   std::string text_;
//   std::string font_name_;
//   glow::vec3 pos_;
//   float scale_;
// };

}  // namespace gl

#endif  // CODE_OPENGL_TUTORIALS_GL_SCENE_DRAWABLES_ALL_H_
