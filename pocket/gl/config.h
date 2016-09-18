#ifndef __POCKET_GL_CONFIG_H__
#define __POCKET_GL_CONFIG_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#if defined(GLEW_STATIC) || defined(__GLEW__) || defined(POCKET_USE_GLEW)
#define POCKET_INTERNAL_USE_GLEW
#endif //

#endif // __POCKET_GL_CONFIG_H__
