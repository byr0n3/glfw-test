#pragma once

// don't remove
#include <GL/glew.h>
#include <cassert>

#define GL_CHECK(x) (x); assert(!glGetError());
