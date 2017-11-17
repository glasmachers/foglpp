#pragma once

#include <fogl/cref.hpp>
#include <fogl/obj.hpp>
#include <fogl/flags.hpp>
#include <fogl/check.hpp>
#include <fogl/error.hpp>
#include <fogl/exception.hpp>
#include <fogl/gl.hpp>

#include <initializer_list>
#include <cassert>

namespace fogl {

  static inline constexpr GLenum buffer_type_to_binding(GLenum type) {
    switch (type) {
      case GL_ARRAY_BUFFER: return GL_ARRAY_BUFFER_BINDING;
      case GL_ELEMENT_ARRAY_BUFFER: return GL_ELEMENT_ARRAY_BUFFER_BINDING;
      default: return 0;
    }
  }

  /// C++ wrapper of a reference to a constant opengl buffer.
  template<GLenum type> struct buffer_cref : cref {
    /// Whether the buffer is bound.
    bool is_bound() const {
      GLint bound_id;
      glGetIntegerv(texture_type_to_binding(type), &bound_id);
      auto_check_error();
      return bound_id == id();
    }
    /// Exception which is thrown if a buffer was not bound.
    struct not_bound : exception {
      GLuint id;
      not_bound(GLuint id) : id(id) {
      }
    };
    /// Checks whether the buffer is bound. If its not, throws not_bound exception.
    void check_bound() const {
      if (!is_bound())
        throw not_bound(id());
    }
    /// If auto state checking is enabled, checks whether the buffer is bound. If its not, throws not_bound exception.
    void auto_check_bound() const {
#ifdef FOGL_AUTO_STATE_CHECKING
      check_bound();
#endif
    }
    /// Bind the buffer.
    void bind() const {
      glBindBuffer(type, id());
      auto_check_error();
    }
    /// Construct with null id.
    buffer_cref() {
    }
    /// Construct from a given id.
    buffer_cref(from_id, GLuint id) : cref(from_id(), id) {
    }
    /// Construct with undefined id.
    buffer_cref(undefined) : cref(undefined()) {
    }
  };

  /// C++ wrapper of a reference to an opengl buffer.
  template<GLenum type> struct buffer_ref : buffer_cref<type> {
    /// Set the data of the buffer.
    void data(const void *buf, size_t size, GLenum usage = GL_STATIC_DRAW) const {
      this->auto_check_not_null();
      this->auto_check_bound();
      glBufferData(type, size, buf, usage);
      auto_check_error();
    }
    /// Set the data of the buffer.
    template<typename t> void data(const std::initializer_list<t> &il, GLenum usage = GL_STATIC_DRAW) const {
      data(il.begin(), il.size() * sizeof(t), usage);
    }
    /// Create with null id.
    buffer_ref() {
    }
    /// Create without checking whether the id is valid.
    /// Create from a given id.
    buffer_ref(from_id, GLuint id) : buffer_cref<type>(from_id(), id) {
    }
    /// Create without checking whether the id is valid.
    buffer_ref(undefined) : buffer_cref<type>(undefined()) {
    }
  };

  /// C++ wrapper of an opengl buffer.
  template<GLenum type> struct buffer : obj<buffer<type>, buffer_ref<type>, buffer_cref<type>> {
    using obj<buffer<type>, buffer_ref<type>, buffer_cref<type>>::id;
    /// Destroy the buffer.
    void destroy() {
      if (this->is_null())
        return;
      GLuint id = this->id();
      glDeleteBuffers(1, &id);
      this->invalidate();
    }
    /// Create the buffer
    void create() {
      GLuint id = 0;
      glGenBuffers(1, &id);
      auto_check_error();
      this->id(id);
    }
    /// Construct with invalid id.
    buffer() {
    }
    /// Construct from a given id.
    buffer(from_id, GLuint id) : obj<buffer<type>, buffer_ref<type>, buffer_cref<type>>(from_id(), id) {
    }
    /// Construct with opengl buffer created
    buffer(struct create) {
      create();
    }
    /// Construct with opengl buffer created and data set
    buffer(const void *buf, size_t size, GLenum usage = GL_STATIC_DRAW) {
      create();
      (*this)->data(buf, size, usage);
    }
    /// Construct with opengl buffer created and data set
    template<typename t> buffer(const std::initializer_list<t> &il, GLenum usage = GL_STATIC_DRAW) {
      create();
      (*this)->data(il, usage);
    }
  };

  /// C++ wrapper of a reference to a constant opengl array buffer.
  using array_buffer_cref = buffer_cref<GL_ARRAY_BUFFER>;
  /// C++ wrapper of a reference to a mutable opengl array buffer.
  using array_buffer_ref = buffer_ref<GL_ARRAY_BUFFER>;
  /// C++ wrapper of an opengl array buffer.
  using array_buffer = buffer<GL_ARRAY_BUFFER>;

  /// C++ wrapper of a reference to a constant opengl element array buffer.
  using element_array_buffer_cref = buffer_cref<GL_ELEMENT_ARRAY_BUFFER>;
  /// C++ wrapper of a reference to a mutable opengl element array buffer.
  using element_array_buffer_ref = buffer_ref<GL_ELEMENT_ARRAY_BUFFER>;
  /// C++ wrapper of an opengl element array buffer.
  using element_array_buffer = buffer<GL_ELEMENT_ARRAY_BUFFER>;

}
