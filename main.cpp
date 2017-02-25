#include "pocket/math/all.h"
#include "pocket/gl/all.h"
#include <GLFW/glfw3.h>
#include <iomanip>
#include <cstddef>

namespace io = pocket::io;
namespace gl = pocket::gl;
namespace math = pocket::math;
namespace call = pocket::call;

// テスト用
// #define __MAIN_TEST

#if POCKET_COMPILER_IF(VC)
#define __FILE_RELATIVE(PATH) "../" PATH
#else
#define __FILE_RELATIVE(PATH) PATH
#endif // POCKET_COMPILER_IF(VC)

class main_lock
{
	GLFWwindow* win;

public:
	main_lock(GLFWwindow* win) :
		win(win)
	{}
	operator GLFWwindow* () const
	{
		return win;
	}
	operator bool() const
	{
		return win != NULL;
	}
	bool operator ! () const
	{
		return win == NULL;
	}
	~main_lock()
	{
		if (win != NULL)
		{
			glfwDestroyWindow(win);
			win = NULL;
		}
		glfwTerminate();
	}
};

struct simple_vertex_t
{
	math::vector3f position;
	math::colorf color;
};
struct ublock_t
{
	math::matrix4x4f world;
	math::matrix4x4f lookat;
	math::matrix4x4f perspective;
};

#ifdef __MAIN_TEST

int main()
{
	math::quaternionf a, b;
	a.from_angle_x(24.0f);
	b.from_angle_x(76.0f);

	math::quaternionf d = a.difference(b);

	std::cout << a << std::endl <<
		b << std::endl <<
		d << std::endl <<
		"angle: " << a.angle(b) << " : " << d.angle() << std::endl;

	math::vector3f ax;
	float an;
	a.axis_angle(ax, an);

	std::cout << ax << " : " << an << std::endl;

	return 0;
}

#else // __MAIN_TEST

static void glfw_error_log(int, const char* msg)
{
	std::cout << msg << std::endl;
	std::exit(EXIT_FAILURE);
}
template <typename T>
static void log(const T& a)
{
	typename T::binder_type binder = a.make_binder();
	std::cout << a << std::endl;
}

int main()
{
	std::cout << std::boolalpha << std::fixed << std::flush;
	std::ios_base::sync_with_stdio(false);

	GLuint err = glfwInit();
	if (err != GL_TRUE)
	{
		return EXIT_FAILURE;
	}
#ifdef POCKET_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // POCKET_DEBUG
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwSetErrorCallback(&glfw_error_log);

	// ウィンドウの破棄と終了処理を自動で呼ぶようにする
	const main_lock lock = glfwCreateWindow(1280, 720, "opengl", NULL, NULL);
	if (!lock)
	{
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(lock);
	glfwSetWindowPos(lock, 4, 28);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	err = glewInit();
	if (err != GLEW_NO_ERROR)
	{
		std::cout << glewGetErrorString(err) << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR_MSG("glew skip error");
	//POCKET_GL_ERROR();

	// デバッグ機能が使えるか
	// std::cout << "GL_ARB_debug_output: " << gl::is_extension_support("GL_ARB_debug_output") << std::endl;

	// デフォルトビューポート取得
	gl::viewport viewport = gl::get_binding_viewport();
	if (!viewport)
	{
		std::cout << viewport << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	std::cout << viewport << std::endl;

	// 頂点シェーダ―作成
	gl::shader vert = gl::make_vertex_shader(__FILE_RELATIVE("test.vert"));
	if (!vert)
	{
		std::cout << vert << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	std::cout << vert << std::endl;

	// フラグメントシェーダー作成
	gl::shader frag = gl::make_fragment_shader(__FILE_RELATIVE("test.frag"));
	if (!frag)
	{
		std::cout << frag << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	std::cout << frag << std::endl;

	// パイプライン管理プログラム作成
	gl::program prog = gl::make_program(vert, frag, true);
	//gl::program prog = gl::make_program("test.shbin", true);
	if (!prog)
	{
		std::cout << prog << std::endl;
		return EXIT_FAILURE;
	}
	// バイナリの保存
	if (!prog.save_binary("test.shbin", true))
	{
		std::cout << "save_binary: " << prog.error() << std::endl;
		prog.clear();
	}
	// UBOの情報を出力
	//prog.reflect_uniform_block(std::cout);
	POCKET_GL_ERROR();
	std::cout << prog << std::endl;

	// ubo用データ構築
	ublock_t data;
	data.world.load_identity();
	data.lookat.load_lookat(math::vector3f(1.0f, 0.5f, 1.5f), math::vector3f::zero, math::vector3f::up);
	data.perspective.load_perspective_field_of_view(45.0f, viewport.aspect(), 0.1f, 100.0f);

	// uniform buffer object作成
	// gl::uniform_buffer ubo = gl::make_uniform_buffer(prog, "ublock", 0, data);
	gl::uniform_buffer ubo = prog.make_uniform_buffer("ublock", 0, data);
	if (!ubo)
	{
		std::cout << ubo << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	std::cout << ubo << std::endl;

	// レイアウトを指定した頂点バッファを作成
	const gl::vertex_layout layouts[] = {
		POCKET_LAYOUT_OFFSETOF(float, 3, false, simple_vertex_t, position),
		POCKET_LAYOUT_OFFSETOF(float, 4, false, simple_vertex_t, color),
	};
	const simple_vertex_t vertices[] = {
		{math::vector3f(0.0f, 0.5f, 0.1f), math::colorf::red},
		{math::vector3f(-0.5f, -0.5f, 0.1f), math::colorf::blue},
		{math::vector3f(0.5f, -0.5f, 0.1f), math::colorf::green},
	};
	gl::layered_vertex_buffer<simple_vertex_t> lvb = gl::make_layered_vertex_buffer(vertices, layouts);
	if (!lvb)
	{
		std::cout << lvb << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	log(lvb);

	// const math::vector3f z(0.0f, 0.01f, 0.0f);
	const simple_vertex_t axis_vertices[] = {
		{math::vector3f::zero, math::colorf::red},
		{math::vector3f(1.0f, 0.0f, 0.0f), math::colorf::red},
		{math::vector3f::zero, math::colorf::blue},
		{math::vector3f(0.0f, 1.0f, 0.0f), math::colorf::blue},
		{math::vector3f::zero, math::colorf::green},
		{math::vector3f(0.0f, 0.0f, 1.0f), math::colorf::green},
	};
	gl::layered_vertex_buffer<simple_vertex_t> axis_lvb = gl::make_layered_vertex_buffer(axis_vertices, layouts);
	if (!axis_lvb)
	{
		std::cout << axis_lvb << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	log(axis_lvb);

	// インダイレクトバッファ作成
	gl::draw_indirect_buffer dib = gl::make_draw_indirect_buffer(gl::command_type::arrays, POCKET_ARRAY_SIZE(vertices));
	if (!dib)
	{
		std::cout << dib << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	log(dib);

	// サンプラー作成
	gl::sampler smpl = gl::make_sampler(gl::wrap_type::clamp_to_edge, gl::filter_type::nearest, gl::compare_func_type::equal);
	if (!smpl)
	{
		std::cout << smpl << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	std::cout << smpl << std::endl;

	// 同期オブジェクト作成
	gl::sync sync = gl::make_sync();
	if (!sync)
	{
		std::cout << sync << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();
	std::cout << sync << std::endl;

	// 受け取るメッセージバッファ
	// char message[256];

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	do
	{
		gl::clear(math::colorf::black, gl::clear_type::all);

		// ubo用データ更新
		float time = static_cast<float>(glfwGetTime());

		// 各種バインド
		viewport.bind();
		prog.bind();
		ubo.bind();

		// -----
		// 軸
		data.world.load_identity();
		ubo.uniform(0, data.world);

		axis_lvb.bind();
		axis_lvb.draw(gl::draw_type::lines);

		// ----
		// 三角形
		typedef math::math_traitsf mt;
		const math::vector3f scale(1.0f);
		const math::quaternionf rotate(math::vector3f::unit_z, mt::to_degree(time));
		const math::vector3f position(0.0f);

		data.world.load_world(scale, rotate, position);
		// data.world.load_rotate_x(mt::to_degree(time));
		ubo.uniform(0, data.world);

		lvb.bind();
		dib.bind();
		lvb.draw(gl::draw_type::triangles, dib);

		// バインド解除
		prog.unbind();
		lvb.unbind();
		dib.unbind();

		// GPU同期
		sync.wait();

		glfwPollEvents();
		glfwSwapBuffers(lock);
	} while ((glfwGetKey(lock, GLFW_KEY_ESCAPE) | glfwWindowShouldClose(lock)) == GL_FALSE);

	sync.finalize();
	smpl.finalize();
	dib.finalize();
	lvb.finalize();
	axis_lvb.finalize();
	ubo.finalize();
	prog.finalize();
	vert.finalize();
	frag.finalize();
}

#ifdef _MSC_VER
#pragma comment (lib, "OpenGL32.lib")
#	ifdef _DEBUG
#		ifdef _MT
#			ifdef _DLL
#pragma comment (lib, "glfw3_mdd.lib")
#pragma comment (lib, "glew32s_mdd.lib")
#			else
#pragma comment (lib, "glfw3_mtd.lib")
#pragma comment (lib, "glew32s_mtd.lib")
#			endif
#		endif
#	else
#		ifdef _MT
#			ifdef _DLL
#pragma comment (lib, "glfw3_md.lib")
#pragma comment (lib, "glew32s_md.lib")
#			else
#pragma comment (lib, "glfw3_mt.lib")
#pragma comment (lib, "glew32s_mt.lib")
#			endif
#		endif
#	endif // _MT
#endif // _MSC_VER

#endif // __MAIN_TEST
