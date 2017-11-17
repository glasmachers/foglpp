#pragma once

#include <fogl/cref.hpp>
#include <fogl/obj.hpp>
#include <fogl/flags.hpp>
#include <fogl/check.hpp>
#include <fogl/error.hpp>
#include <fogl/exception.hpp>
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
    /// Whether the texture is bound.
    bool is_bound() const {
      GLint bound_id;
      glGetIntegerv(texture_type_to_binding(type), &bound_id);
      return bound_id == id();
    }
    /// Exception which is thrown if a texture was not bound.
    struct not_bound : exception {
      GLuint id;
      not_bound(GLuint id) : id(id) {
      }
    };
    /// Checks whether the texture is bound. If its not, throws not_bound exception.
    void check_bound() const {
      if (!is_bound())
        throw not_bound(this->id());
    }
    /// If auto state checking is enabled, checks whether the texture is bound. If its not, throws not_bound exception.
    void auto_check_bound() const {
#ifdef FOGL_AUTO_STATE_CHECKING
      check_bound();
#endif
    }
    /// Bind the texture
    void bind() const {
      glBindTexture(type, id());
      auto_check_error();
    }
    /// Construct with null id
    texture_cref() {
    }
    /// Construct from a given id.
    texture_cref(from_id, GLuint id) : cref(from_id(), id) {
    }
    /// Construct with undefined id.
    texture_cref(undefined) : cref(undefined()) {
    }
  };

  /// C++ wrapper of a reference to a mutable opengl texture.
  template<GLenum type> struct texture_ref : texture_cref<type> {
    using texture_cref<type>::id;
    /// Set 2d image data of the texture.
    void img2d(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type_, const GLvoid *data) const {
      this->auto_check_not_null();
      this->auto_check_bound();
      glTexImage2D(type, level, internalFormat, width, height, 0, format, type_, data);
      auto_check_error();
    }
    void param(GLenum pname, GLint param) const {
      this->auto_check_not_null();
      this->auto_check_bound();
      glTexParameteri(type, pname, param);
      auto_check_error();
    }
    void wrap_s(GLint param) const {
      this->param(GL_TEXTURE_WRAP_S, param);
    }
    void wrap_t(GLint param) const {
      this->param(GL_TEXTURE_WRAP_T, param);
    }
    void wrap_s_t(GLint param) const {
      wrap_s(param);
      wrap_t(param);
    }
    void mag_filter(GLint param) const {
      this->param(GL_TEXTURE_MAG_FILTER, param);
    }
    void min_filter(GLint param) const {
      this->param(GL_TEXTURE_MIN_FILTER, param);
    }
    void min_mag_filter(GLint param) const {
      mag_filter(param);
      min_filter(param);
    }
    void gen_mipmaps() const {
      this->auto_check_not_null();
      this->auto_check_bound();
      glGenerateMipmap(type);
      auto_check_error();
    }
    /// Construct with null id
    texture_ref() {
    }
    /// Construct from a given id.
    texture_ref(from_id, GLuint id) : texture_cref<type>(from_id(), id) {
    }
    /// Construct without checking whether the id is valid.
    texture_ref(undefined) : texture_cref<type>(undefined()) {
    }
  };

  /// C++ wrapper for an opengl texture.
  template<GLenum type> struct texture : obj<texture<type>, texture_ref<type>, texture_cref<type>> {
    /// Destroy the texture.
    void destroy() {
      if (this->is_null())
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
    texture(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type_, const GLvoid *data) {
      create();
      (*this)->img2d(level, internalFormat, width, height, format, type_, data);
    }
  };

  /// C++ wrapper of a reference to a constant opengl array buffer.
  using texture2d_cref = texture_cref<GL_TEXTURE_2D>;
  /// C++ wrapper of a reference to a mutable opengl array buffer.
  using texture2dr_ref = texture_ref<GL_TEXTURE_2D>;
  /// C++ wrapper of an 2d opengl texture.
  using texture2d = texture<GL_TEXTURE_2D>;

}
