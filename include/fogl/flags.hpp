#pragma once

#include <fogl/gl.hpp>

namespace fogl {

  /// Used as a flag to construct with a given opengl id.
  struct from_id {};
  /// Used as a flag to construct an object by creating the wrapped opengl object.
  struct create {};
  /// Used as a flag to construct a reference with an undefined id.
  struct undefined {};

}
