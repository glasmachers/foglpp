#pragma once

#include <fogl/flags.hpp>
#include <fogl/gl.hpp>

#include <cassert>

namespace fogl {

  struct obj_base {
  protected:
    template<typename ref> static GLuint &ref_id(ref &r) {
      return r.id();
    }
  };

  /// Base class of all C++ wrappers of an opengl object.
  template<typename child, typename ref, typename cref> struct obj : obj_base {
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
      o.id() = 0;
      return *this;
    }
    /// Create with invalid id.
    obj() : ref_(no_validity_check(), 0) {
    }
    /// Create from a given id.
    obj(from_id, GLuint id) : ref_(no_validity_check(), id) {
    }
    /// Create from a given id.
    obj(std::nullptr_t) : ref_(no_validity_check(), 0) {
    }
    obj(const obj<child, ref, cref>&) = delete;
    obj(obj<child, ref, cref>&& o) : ref_(no_validity_check(), o.id()) {
      o.id() = 0;
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
      ref_.id(no_validity_check(), id);
    }
    void invalidate() {
      ref_.id(no_validity_check(), 0);
    }
    /// Whether the id is valid.
    operator bool() const {
      return id() != 0;
    }
    /// Convert to pointer.
    operator cptr<ref, cref>() const {
      return {from_id(), id()};
    }
    /// Convert to pointer.
    operator ptr<ref, cref>() {
      return {from_id(), id()};
    }
    /// Dereference.
    const cref *operator->() const {
      assert(*this);
      return &ref_;
    }
    /// Dereference.
    cref operator*() const {
      assert(*this);
      return ref_;
    }
    /// Dereference.
    const ref *operator->() {
      assert(*this);
      return &ref_;
    }
    /// Dereference.
    ref operator*() {
      assert(*this);
      return ref_;
    }
    /// Whether the id is invalid.
    bool operator==(std::nullptr_t) const {
      return id() == 0;
    }
  };
}
