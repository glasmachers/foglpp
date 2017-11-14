#pragma once

#include <fogl/cptr.hpp>
#include <fogl/flags.hpp>
#include <fogl/gl.hpp>

#include <cassert>

namespace fogl {

  /// C++ wrapper of a pointer to a mutable opengl object.
  template<typename ref, typename cref> struct ptr {
  private:
    ref ref_;
  public:
    /// Get the id.
    GLuint id() const {
      return ref_.id();
    }
    /// Set the id.
    void id(GLuint id) {
      ref_.id(no_validity_check(), id);
    }
    /// Whether the id is valid.
    operator bool() const {
      return id() != 0;
    }
    /// Convert to cptr
    operator cptr<ref, cref>() const {
      return {from_id(), id()};
    }
    /// Dereference.
    const ref *operator->() const {
      assert(*this);
      return &ref_;
    }
    /// Dereference.
    ref operator*() const {
      assert(*this);
      return ref_;
    }
    /// Whether the id is invalid.
    bool operator==(std::nullptr_t) const {
      return id() == 0;
    }
    /// Invalidate the id.
    ptr &operator=(std::nullptr_t) {
      id() = 0;
      return *this;
    }
    /// Create with undefined id.
    ptr() : ref_(undefined()) {
    }
    /// Create with invalid id.
    ptr(std::nullptr_t) : ref_(no_validity_check(), 0) {
    }
    /// Create from a given id.
    ptr(from_id, GLuint id) : ref_(no_validity_check(), id) {
    }
  };

}
