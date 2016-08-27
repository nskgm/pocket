#ifndef __POCKET_GL_FWD_H__
#define __POCKET_GL_FWD_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

namespace pocket
{
namespace gl
{

namespace commands
{
struct draw_arrays;
struct draw_elements;
struct dispatch_compute;
}

template <typename> class binder;
template <typename, typename> class binder1;
template <typename, typename> class binder_map;

class buffer;
class buffer_view;
template <int> class buffers;
class shader;
class program;
class uniform_buffer;

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_FWD_H__
