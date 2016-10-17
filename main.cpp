#include "pocket/math/all.h"
#include "pocket/gl/all.h"
#include <GLFW/glfw3.h>
#include <iomanip>
#include <cstddef>

namespace io = pocket::io;
namespace gl = pocket::gl;
namespace math = pocket::math;

class main_lock
{
	GLFWwindow* win;

public:
	explicit main_lock(GLFWwindow* win) :
		win(win)
	{}
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

static void glfw_error_log(int, const char* msg)
{
	std::cout << msg << std::endl;
	std::exit(EXIT_FAILURE);
}

struct simple_vertex
{
	math::vector3f position;
	math::colorf color;
};
struct ublock
{
	math::matrix4x4f world;
	math::matrix4x4f lookat;
	math::matrix4x4f perspective;
};

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

	GLFWwindow* window = glfwCreateWindow(640, 480, "opengl", NULL, NULL);
	// ウィンドウの破棄と終了処理を自動で呼ぶようにする
	const main_lock lock(window);
	if (window == NULL)
	{
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 4, 28);
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

	// 頂点シェーダ―作成
	gl::shader vert = gl::make_vertex_shader("test.vert");
	if (!vert)
	{
		std::cout << vert << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();

	// フラグメントシェーダー作成
	gl::shader frag = gl::make_fragment_shader("test.frag");
	if (!frag)
	{
		std::cout << frag << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();

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

	// ubo用データ構築
	ublock data;
	data.world.load_identity();
	data.lookat.load_lookat(math::vector3f(2.0f), math::vector3f::zero, math::vector3f::up);
	data.perspective.load_perspective_field_of_view(45.0f, viewport.aspect(), 0.1f, 100.0f);

	// uniform buffer object作成
	gl::uniform_buffer ubo = gl::make_uniform_buffer(prog, "ublock", 0, data);
	if (!ubo)
	{
		std::cout << ubo << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();

	// レイアウトを指定した頂点バッファを作成
	const gl::vertex_layout layouts[] = {
		POCKET_LAYOUT_OFFSETOF(float, 3, false, simple_vertex, position),
		POCKET_LAYOUT_OFFSETOF(float, 4, false, simple_vertex, color),
	};
	const simple_vertex vertices[] = {
		{math::vector3f(0.0f, 0.5f, 0.0f), math::colorf::red},
		{math::vector3f(-0.5f, -0.5f, 0.0f), math::colorf::blue},
		{math::vector3f(0.5f, -0.5f, 0.0f), math::colorf::green}
	};
	gl::layered_vertex_buffer<simple_vertex> lvb = gl::make_layered_vertex_buffer(vertices, layouts);
	if (!lvb)
	{
		std::cout << lvb << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();

	// インダイレクトバッファ作成
	gl::draw_indirect_buffer dib = gl::make_draw_indirect_buffer(gl::command_type::arrays, POCKET_ARRAY_SIZE(vertices));
	if (!dib)
	{
		std::cout << dib << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();

	// サンプラー作成
	gl::sampler smpl = gl::make_sampler(gl::wrap_type::clamp_to_edge, gl::filter_type::nearest, gl::compare_func_type::equal);
	if (!smpl)
	{
		std::cout << smpl << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();

	// 同期オブジェクト作成
	gl::sync sync = gl::make_sync();
	if (!sync)
	{
		std::cout << sync << std::endl;
		return EXIT_FAILURE;
	}
	POCKET_GL_ERROR();

	glfwSetTime(0.0f);

	// 受け取るメッセージバッファ
	char message[256];
	math::quaternionf quat;

	do
	{
		gl::clear(math::colorf::black, gl::clear_type::all);

		// ubo用データ更新
		float time = static_cast<float>(glfwGetTime());
		data.world.load_world(math::vector3f::one*0.5f,
			quat.from_axis(math::vector3f::unit_z, math::math_traitsf::to_degree(time*10.0f)),
			math::vector3f(math::math_traitsf::sin(math::math_traitsf::to_degree(time*1.5f)), 0.0f, 0.0f));

		ubo.uniform(0, data.world); // オフセットを指定して指定した型サイズのみ更新
		//ubo.uniform(data); // 全体の更新

		// 各種バインド
		prog.bind();
		ubo.bind();
		lvb.bind();
		dib.bind();

		// バインドされている状態で描画できる状態か確認
		if (!prog.drawable(message))
		{
			std::cout << message << std::endl;
			break;
		}

		// 描画
		lvb.draw(gl::draw_type::triangles, dib);

		// バインド解除
		prog.unbind();
		lvb.unbind();
		dib.unbind();

		// GPU同期
		sync.wait();

		glfwPollEvents();
		glfwSwapBuffers(window);
	} while ((glfwGetKey(window, GLFW_KEY_ESCAPE) | glfwWindowShouldClose(window)) == GL_FALSE);

	sync.finalize();
	smpl.finalize();
	dib.finalize();
	lvb.finalize();
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
