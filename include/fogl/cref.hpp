#pragma once

#include <fogl/flags.hpp>
#include <fogl/gl.hpp>

#include <cassert>

namespace fogl {

  struct cref {
  private:
    /// The opengl id.
    GLuint id_;
  protected:
    /// Create with undefined id.
    cref(undefined) {
    }
    /// Create from a given id.
    cref(from_id, GLuint id) : id_(id) {
      assert(id_);
    }
    /// Create without checking whether the id is valid.
    cref(no_validity_check, GLuint id) : id_(id) {
    }
  public:
    /// Get the id.
    GLuint id() const {
      return id_;
    }
    /// Set the id.
    void id(GLuint id) {
      assert(id);
      id_ = id;
    }
    /// Set the id without checking whether it is valid.
    void id(no_validity_check, GLuint id) {
      id_ = id;
    }
    /// Convert to id.
    operator GLuint() const {
      return id();
    }
  };

}
