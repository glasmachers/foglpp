#pragma once

#include <fogl/cref.hpp>
#include <fogl/cptr.hpp>
#include <fogl/ptr.hpp>
#include <fogl/obj.hpp>
#include <fogl/flags.hpp>
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
    /// Bind the buffer.
    void bind() const {
      glBindBuffer(type, id());
    }
    /// Create from a given id.
    buffer_cref(from_id, GLuint id) : cref(from_id(), id) {
    }
    /// Create without checking whether the id is valid.
    buffer_cref(no_validity_check, GLuint id) : cref(no_validity_check(), id) {
    }
    /// Create with undefined id.
    buffer_cref(undefined) : cref(undefined()) {
    }
  };

  /// C++ wrapper of a reference to an opengl buffer.
  template<GLenum type> struct buffer_ref : buffer_cref<type> {
    using buffer_cref<type>::id;
    /// Set the data of the buffer.
    void data(const void *buf, size_t size, GLenum usage = GL_STATIC_DRAW) const {
      GLint prev_bound_id;
      glGetIntegerv(buffer_type_to_binding(type), &prev_bound_id);
      glBindBuffer(type, id());
      glBufferData(type, size, buf, usage);
      assert(glGetError() == 0);
      glBindBuffer(type, prev_bound_id);
    }
    /// Set the data of the buffer.
    template<typename t> void data(const std::initializer_list<t> &il, GLenum usage = GL_STATIC_DRAW) const {
      data(il.begin(), il.size() * sizeof(t), usage);
    }
    /// Create from a given id.
    buffer_ref(from_id, GLuint id) : buffer_cref<type>(from_id(), id) {
    }
    /// Create without checking whether the id is valid.
    buffer_ref(no_validity_check, GLuint id) : buffer_cref<type>(no_validity_check(), id) {
    }
    /// Create without checking whether the id is valid.
    buffer_ref(undefined) : buffer_cref<type>(undefined()) {
    }
  };

  /// C++ wrapper of a pointer to a constant opengl buffer.
  template<GLenum type> using buffer_cptr = cptr<buffer_ref<type>, buffer_cref<type>>;
  /// C++ wrapper of a pointer to a mutable opengl buffer.
  template<GLenum type> using buffer_ptr = ptr<buffer_ref<type>, buffer_cref<type>>;

  /// C++ wrapper of an opengl buffer.
  template<GLenum type> struct buffer : obj<buffer<type>, buffer_ref<type>, buffer_cref<type>> {
    using obj<buffer<type>, buffer_ref<type>, buffer_cref<type>>::id;
    /// Destroy the buffer.
    void destroy() {
      if (!*this)
        return;
      GLuint id = this->id();
      glDeleteBuffers(1, &id);
      this->invalidate();
    }
    /// Create the buffer
    void create() {
      GLuint id = 0;
      glGenBuffers(1, &id);
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
    buffer(struct create, const void *buf, size_t size, GLenum usage = GL_STATIC_DRAW) {
      create();
      (*this)->data(buf, size, usage);
    }
    /// Construct with opengl buffer created and data set
    template<typename t> buffer(struct create, const std::initializer_list<t> &il, GLenum usage = GL_STATIC_DRAW) {
      create();
      (*this)->data(il, usage);
    }
  };

  /// C++ wrapper of a reference to a constant opengl array buffer.
  using array_buffer_cref = buffer_cref<GL_ARRAY_BUFFER>;
  /// C++ wrapper of a reference to a mutable opengl array buffer.
  using array_buffer_ref = buffer_ref<GL_ARRAY_BUFFER>;
  /// C++ wrapper of a pointer to a constant opengl array buffer.
  using array_buffer_cptr = buffer_cptr<GL_ARRAY_BUFFER>;
  /// C++ wrapper of a pointer to a mutable opengl array buffer.
  using array_buffer_ptr = buffer_ptr<GL_ARRAY_BUFFER>;
  /// C++ wrapper of an opengl array buffer.
  using array_buffer = buffer<GL_ARRAY_BUFFER>;

  /// C++ wrapper of a reference to a constant opengl element array buffer.
  using element_array_buffer_cref = buffer_cref<GL_ELEMENT_ARRAY_BUFFER>;
  /// C++ wrapper of a reference to a mutable opengl element array buffer.
  using element_array_buffer_ref = buffer_ref<GL_ELEMENT_ARRAY_BUFFER>;
  /// C++ wrapper of a pointer to a constant opengl element array buffer.
  using element_array_buffer_cptr = buffer_cptr<GL_ELEMENT_ARRAY_BUFFER>;
  /// C++ wrapper of a pointer to a mutable opengl element array buffer.
  using element_array_buffer_ptr = buffer_ptr<GL_ELEMENT_ARRAY_BUFFER>;
  /// C++ wrapper of an opengl element array buffer.
  using element_array_buffer = buffer<GL_ELEMENT_ARRAY_BUFFER>;

}
