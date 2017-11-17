#pragma once

#include <fogl/cref.hpp>
#include <fogl/obj.hpp>
#include <fogl/flags.hpp>
#include <fogl/check.hpp>
#include <fogl/error.hpp>
#include <fogl/exception.hpp>
#include <fogl/gl.hpp>

#include <initializer_list>
#include <string>
#include <vector>
#include <cassert>

namespace fogl {

  /// C++ wrapper of a reference to a constant opengl shader.
  template<GLenum type> struct shader_cref : cref {
    /// Status of the shader.
    bool status() const {
      this->auto_check_not_null();
      GLint res = 0;
      glGetShaderiv(this->id(), GL_COMPILE_STATUS, &res);
      return res != 0;
    }
    /// Log of the shader.
    std::string log() const {
      this->auto_check_not_null();
      int len;
      glGetShaderiv(this->id(), GL_INFO_LOG_LENGTH, &len);
      std::vector<char> buf(len + 1);
      if (len > 0)
        glGetShaderInfoLog(this->id(), len, NULL, &buf[0]);
      buf[len] = 0;
      return std::string(&buf[0]);
    }
    /// Construct with null id.
    shader_cref() {
    }
    /// Construct from a given id.
    shader_cref(from_id, GLuint id) : cref(from_id(), id) {
    }
    /// Construct with undefined id.
    shader_cref(undefined) : cref(undefined()) {
    }
  };

  /// C++ wrapper of a reference to a mutable opengl shader.
  template<GLenum type> struct shader_ref : shader_cref<type> {
    /// Set the shader source.
    void src(std::initializer_list<const char *> src) const {
      this->auto_check_not_null();
      std::string csrc;
      for (const char *s : src) {
        csrc += s;
      };
      const char *s = csrc.c_str();
      glShaderSource(this->id(), 1, &s, NULL);
      auto_check_error();
    }
    /// Compile the shader.
    void compile() const {
      this->auto_check_not_null();
      glCompileShader(this->id());
      auto_check_error();
    }
    /// Construct with null id.
    shader_ref() {
    }
    /// Construct from a given id.
    shader_ref(from_id, GLuint id) : shader_cref<type>(from_id(), id) {
    }
    /// Construct with undefined id.
    shader_ref(undefined) : shader_cref<type>(undefined()) {
    }
  };

  /// C++ wrapper of an opengl shader.
  template<GLenum type> struct shader : obj<shader<type>, shader_ref<type>, shader_cref<type>> {
    /// Destroy the shader.
    void destroy() {
      if (!*this)
        return;
      glDeleteShader(this->id());
      this->invalidate();
    }
    /// Create the shader
    void create() {
      this->id(glCreateShader(type));
      auto_check_error();
    }
    /// Set the shader source.
    void src(std::initializer_list<const char *> src) {
      assert(!this->is_null());
      std::string csrc;
      for (const char *s : src) {
        csrc += s;
      };
      const char *s = csrc.c_str();
      glShaderSource(this->id(), 1, &s, NULL);
      assert(glGetError() == 0);
    }
    /// Compile the shader.
    void compile() {
      assert(!this->is_null());
      glCompileShader(this->id());
      assert(glGetError() == 0);
    }
    /// Construct with null id.
    shader() {
    }
    /// Construct from a given id.
    shader(from_id, GLuint id) : obj<shader<type>, shader_ref<type>, shader_cref<type>>(from_id(), id) {
    }
    /// Construct with opengl buffer created
    shader(struct create) {
      create();
    }
    /// Construct with opengl buffer created
    shader(const std::initializer_list<const char *>& src) {
      create();
      (*this)->src(src);
      (*this)->compile();
    }
  };

  /// C++ wrapper of an opengl vertex shader.
  using vertex_shader = shader<GL_VERTEX_SHADER>;
  using vertex_shader_ref = shader_ref<GL_VERTEX_SHADER>;
  using vertex_shader_cref = shader_cref<GL_VERTEX_SHADER>;
  /// C++ wrapper of an opengl fragment shader.
  using fragment_shader = shader<GL_FRAGMENT_SHADER>;
  using fragment_shader_ref = shader_ref<GL_FRAGMENT_SHADER>;
  using fragment_shader_cref = shader_cref<GL_FRAGMENT_SHADER>;

}
