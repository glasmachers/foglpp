#pragma once

#include <fogl/flags.hpp>
#include <fogl/gl.hpp>

#include <cassert>

namespace fogl {

  /// Base class of all C++ wrappers of an opengl object.
  template<typename child, typename ref, typename cref> struct obj {
  private:
    ref ref_;
  protected:
    /// Invalidate the id.
    obj<child, ref, cref> &operator=(std::nullptr_t) {
      reinterpret_cast<child*>(this)->destroy();
      return *this;
    }
    obj<child, ref, cref>& operator=(const obj<child, ref, cref>&) = delete;
    obj<child, ref, cref>& operator=(obj<child, ref, cref>&& o) {
      reinterpret_cast<child*>(this)->destroy();
      id() = o.id();
      o.invalidate();
      return *this;
    }
    /// Create with null id.
    obj() {
    }
    /// Create from a given id.
    obj(from_id, GLuint id) : ref_(from_id(), id) {
    }
    obj(const obj<child, ref, cref>&) = delete;
    obj(obj<child, ref, cref>&& o) : ref_(from_id(), o.id()) {
      o.invalidate();
    }
    ~obj() {
      reinterpret_cast<child*>(this)->destroy();
    }
  public:
    /// Get the id.
    GLuint id() const {
      return ref_.id();
    }
    /// Set the id.
    void id(GLuint id) {
      reinterpret_cast<child*>(this)->destroy();
      ref_.id(id);
    }
    /// Invalidate withour of destroying.
    void invalidate() {
      ref_.id(0);
    }
    /// Whether the id is not null.
    operator bool() const {
      return id() != 0;
    }
    /// Whether the id is null.
    bool is_null() const {
      return id() == 0;
    }
    /// Dereference.
    const cref *operator->() const {
      return &ref_;
    }
    /// Dereference.
    cref operator*() const {
      return ref_;
    }
    /// Dereference.
    const ref *operator->() {
      return &ref_;
    }
    /// Dereference.
    ref operator*() {
      return ref_;
    }
    /// Whether the id is invalid.
    bool operator==(std::nullptr_t) const {
      return id() == 0;
    }
  };
}
