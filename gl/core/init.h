// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef CODE_OPENGL_TUTORIALS_GL_CORE_INIT_H_
#define CODE_OPENGL_TUTORIALS_GL_CORE_INIT_H_

#include "glog/logging.h"
#include "third_party/glad/glad.h"

#include <functional>

namespace gl {

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
  if (type == GL_DEBUG_TYPE_ERROR) {
    LOG(FATAL) << "GL ERROR message: '" << message << "'";
  }
}

template <typename FunctionT>
void InitializeGlContext(FunctionT intialization_function) {
  if (!gladLoadGLLoader(
          reinterpret_cast<GLADloadproc>(intialization_function))) {
    LOG(FATAL) << "Cannot initialize OpenGL context with GLAD.";
  }
#ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
#endif
}

}  // namespace gl

#endif  // CODE_OPENGL_TUTORIALS_GL_CORE_INIT_H_
