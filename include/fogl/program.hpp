#pragma once

#include <fogl/shader.hpp>
#include <fogl/cref.hpp>
#include <fogl/obj.hpp>
#include <fogl/flags.hpp>
#include <fogl/gl.hpp>

#include <cassert>

namespace fogl {

  /// C++ wrapper of a reference to a constant opengl program.
  struct program_cref : cref {
    /// Status of the program.
    bool status() const {
      assert(!this->is_null());
      GLint res = 0;
      glGetProgramiv(id(), GL_LINK_STATUS, &res);
      return res != 0;
    }
    /// Log of the program.
    std::string log() const {
      assert(!this->is_null());
      int len;
      glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &len);
      std::vector<char> buf(len + 1);
      if (len > 0)
        glGetProgramInfoLog(id(), len, NULL, &buf[0]);
      buf[len] = 0;
      return std::string(&buf[0]);
    }
    /// Attribute location by name.
    GLuint attribute_location(char const *name) const {
      assert(!this->is_null());
      return glGetAttribLocation(id(), name);
    }
    /// Uniform location by name.
    GLuint uniform_location(char const *name) const {
      assert(!this->is_null());
      return glGetUniformLocation(id(), name);
    }
    /// Use the program.
    void use() const {
      glUseProgram(id());
    }
    /// Construct with null id.
    program_cref() {
    }
    /// Construct from a given id.
    program_cref(from_id, GLuint id) : cref(from_id(), id) {
    }
    /// Construct with undefined id.
    program_cref(undefined) : cref(undefined()) {
    }
  };

  /// C++ wrapper of a reference to a mutable opengl program.
  struct program_ref : program_cref {
    /// Attach a shader to the program.
    template<GLenum type> void attach_shader(shader_ref<type> s) const {
      assert(!this->is_null());
      glAttachShader(id(), s.id());
      assert(glGetError() == 0);
    }
    /// Detach a shader from the program.
    template<GLenum type> void detach_shader(shader_ref<type> s) const {
      assert(!this->is_null());
      glDetachShader(id(), s.id());
      assert(glGetError() == 0);
    }
    /// Link the attached shaders.
    void link() const {
      assert(!this->is_null());
      glLinkProgram(id());
      assert(glGetError() == 0);
    }
    /// Construct with null id.
    program_ref() {
    }
    /// Construct from a given id.
    program_ref(from_id, GLuint id) : program_cref(from_id(), id) {
    }
    /// Construct with undefined id.
    program_ref(undefined) : program_cref(undefined()) {
    }
  };

  /// C++ wrapper of an opengl program.
  struct program : obj<program, program_ref, program_cref> {
    /// Destroy the program.
    void destroy() {
      if (this->is_null())
        return;
      glDeleteProgram(id());
      invalidate();
    }
    /// Create the program
    void create() {
      id(glCreateProgram());
    }
    /// Construct with null id.
    program() {
    }
    /// Construct from a given id.
    program(from_id, GLuint id) : obj<program, program_ref, program_cref>(from_id(), id) {
    }
    /// Construct with opengl buffer created
    program(struct create) {
      create();
    }
    /// Construct with opengl buffer created
    program(vertex_shader_ref& vs, fragment_shader_ref fs) {
      create();
      (*this)->attach_shader(vs);
      (*this)->attach_shader(fs);
      (*this)->link();
    }
  };

}
