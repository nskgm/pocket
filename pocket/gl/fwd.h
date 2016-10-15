#ifndef __POCKET_GL_FWD_H__
#define __POCKET_GL_FWD_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

namespace pocket
{
namespace gl
{

struct draw_arrays_cmd;
struct draw_elements_cmd;
struct dispatch_compute_cmd;

template <typename> class binder;
template <typename, typename> class binder1;
template <typename, typename> class binder_map;

struct buffer_type;
struct buffer_usage_type;
struct buffer_map_type;
struct buffer_binding_type;

class buffer;
class buffer_view;
template <int> class buffers;
class shader;
class program;
class uniform_buffer;
class vertex_array;
template <typename> class vertex_buffer;
template <typename> class index_buffer;
template <typename> class layered_vertex_buffer;
class draw_indirect_buffer;
class sampler;
class sync;
struct viewport;
struct depth_range;

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_FWD_H__
