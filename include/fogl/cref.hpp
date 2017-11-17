#pragma once

#include <fogl/flags.hpp>
#include <fogl/check.hpp>
#include <fogl/error.hpp>
#include <fogl/exception.hpp>
#include <fogl/gl.hpp>

namespace fogl {

  struct cref {
  private:
    /// The opengl id.
    GLuint id_;
  protected:
    /// Construct width null id.
    cref() : id_(0) {
    }
    /// Construct with undefined id.
    cref(undefined) {
    }
    /// Construct from a given id.
    cref(from_id, GLuint id) : id_(id) {
    }
  public:
    /// Get the id.
    GLuint id() const {
      return id_;
    }
    /// Set the id.
    void id(GLuint id) {
      id_ = id;
    }
    /// Convert to id.
    operator GLuint() const {
      return id();
    }
    /// Whether the id is null.
    bool is_null() const {
      return id_ == 0;
    }
    /// Whether the id is not null.
    operator bool() const {
      return id_ != 0;
    }
    /// Check whether the id is not null. If it is null, throw a null_id exception.
    void check_not_null() const {
      if (is_null())
        throw null_id();
    }
    /// If auro null checking is enabled, check whether the id is not null. If it is null, throw a null_id exception.
    void auto_check_not_null() const {
#ifdef FOGL_AUTO_NULL_CHECKING
      check_not_null();
#endif
    }
  };

}
