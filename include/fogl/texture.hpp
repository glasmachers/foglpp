#pragma once

#include <fogl/gl.hpp>

#include <cassert>

namespace fogl {

  static inline constexpr GLenum texture_type_to_binding(GLenum type) {
    switch (type) {
      case GL_TEXTURE_2D: return GL_TEXTURE_BINDING_2D;
      default: return 0;
    }
  }

  /// C++ wrapper of a reference to a constant opengl texture.
  template<GLenum type> struct texture_cref : cref {
    /// Bind the texture
    void bind() const {
      glBindTexture(type, id());
    }
    /// Create from a given id.
    texture_cref(from_id, GLuint id) : cref(from_id(), id) {
    }
    /// Create without checking whether the id is valid.
    texture_cref(no_validity_check, GLuint id) : cref(no_validity_check(), id) {
    }
    /// Create with undefined id.
    texture_cref(undefined) : cref(undefined()) {
    }
  };

  /// C++ wrapper of a reference to a mutable opengl texture.
  template<GLenum type> struct texture_ref : texture_cref<type> {
    using texture_cref<type>::id;
    /// Set 2d image data of the texture.
    void img2d(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type_, const GLvoid *data) const {
      GLint prev_bound_id;
      glGetIntegerv(texture_type_to_binding(type), &prev_bound_id);
      glBindTexture(type, id());
      glTexImage2D(type, level, internalFormat, width, height, 0, format, type_, data);
      assert(glGetError() == 0);
      glBindTexture(type, prev_bound_id);
    }
    void set_params_edge_linear() const {
      GLint prev_bound_id;
      glGetIntegerv(texture_type_to_binding(type), &prev_bound_id);
      glBindTexture(type, id());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      assert(glGetError() == 0);
      glBindTexture(type, prev_bound_id);
    }
    void gen_mipmaps() const {
      GLint prev_bound_id;
      glGetIntegerv(texture_type_to_binding(type), &prev_bound_id);
      glBindTexture(type, id());
      glGenerateMipmap(GL_TEXTURE_2D);
      assert(glGetError() == 0);
      glBindTexture(type, prev_bound_id);
    }
    /// Create from a given id.
    texture_ref(from_id, GLuint id) : texture_cref<type>(from_id(), id) {
    }
    /// Create without checking whether the id is valid.
    texture_ref(no_validity_check, GLuint id) : texture_cref<type>(no_validity_check(), id) {
    }
    /// Create without checking whether the id is valid.
    texture_ref(undefined) : texture_cref<type>(undefined()) {
    }
  };

  /// C++ wrapper of a pointer to a constant opengl texture.
  template<GLenum type> using texture_cptr = cptr<texture_ref<type>, texture_cref<type>>;
  /// C++ wrapper of a pointer to a mutable opengl texture.
  template<GLenum type> using texture_ptr = ptr<texture_ref<type>, texture_cref<type>>;

  /// C++ wrapper for an opengl texture.
  template<GLenum type> struct texture : obj<texture<type>, texture_ref<type>, texture_cref<type>> {
    /// Destroy the texture.
    void destroy() {
      if (!*this)
        return;
      GLuint id = this->id();
      glDeleteTextures(1, &id);
      this->invalidate();
    }
    /// Create the texture
    void create() {
      GLuint id;
      glGenTextures(1, &id);
      this->id(id);
    }
    /// Construct with invalid id.
    texture() {
    }
    /// Construct from a given id.
    texture(from_id, GLuint id) : obj<texture<type>, texture_ref<type>, texture_cref<type>>(from_id(), id) {
    }
    /// Construct with opengl buffer created
    texture(struct create) {
      create();
    }
    /// Construct with opengl buffer created and data set
    texture(struct create, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type_, const GLvoid *data) {
      create();
      (*this)->img2d(level, internalFormat, width, height, format, type_, data);
    }
  };

  /// C++ wrapper of a reference to a constant opengl array buffer.
  using texture2d_cref = texture_cref<GL_TEXTURE_2D>;
  /// C++ wrapper of a reference to a mutable opengl array buffer.
  using texture2dr_ref = texture_ref<GL_TEXTURE_2D>;
  /// C++ wrapper of a pointer to a constant opengl array buffer.
  using texture2d_cptr = texture_cptr<GL_TEXTURE_2D>;
  /// C++ wrapper of a pointer to a mutable opengl array buffer.
  using texture2d_ptr = texture_ptr<GL_TEXTURE_2D>;
  /// C++ wrapper of an 2d opengl texture.
  using texture2d = texture<GL_TEXTURE_2D>;

}
