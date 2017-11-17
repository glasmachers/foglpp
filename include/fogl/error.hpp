#pragma once

#include <fogl/check.hpp>
#include <fogl/exception.hpp>
#include <fogl/gl.hpp>

namespace fogl {

  /// Exception which is thrown if an opengl error occurres.
  struct error : exception {
    // TODO: Include some useful information
  };

  /// Checks whether there was a OpenGL error. If so, throws an error exception.
  static inline void check_error() {
    if (glGetError() != 0)
      throw error();
  }

  /// If error checking is enabled, checks whether there was a OpenGL error. If so, throws an error exception.
  static inline void auto_check_error() {
#ifdef FOGL_AUTO_ERROR_CHECKING
    check_error();
#endif
  }

}
