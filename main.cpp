#include "pocket/math/all.h"
#include "pocket/gl/all.h"
#include <GLFW/glfw3.h>
#include <iomanip>

namespace io = pocket::io;
namespace gl = pocket::gl;
namespace math = pocket::math;

class main_lock
{
public:
	explicit main_lock(GLFWwindow* win) :
		win(win)
	{

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

private:
	GLFWwindow* win;
};

static void glfw_error_log(int, const char* msg)
{
	std::cout << msg << std::endl;
}

struct simple_vertex
{
	math::vector2f position;
	math::colorf color;
};
struct ublock
{
	math::matrix4x4f world;
	math::matrix4x4f ortho;
};

int main()
{
	std::cout << std::boolalpha << std::fixed << std::flush;
	std::ios_base::sync_with_stdio(false);

	GLuint err = glfwInit();
	if (err != GL_TRUE)
	{
		return 1;
	}
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwSetErrorCallback(&glfw_error_log);

	GLFWwindow* window = glfwCreateWindow(640, 480, "test", NULL, NULL);
	const main_lock lock(window);
	if (window == NULL)
	{
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 4, 28);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	err = glewInit();
	if (err != GLEW_NO_ERROR)
	{
		std::cout << glewGetErrorString(err) << std::endl;
		return 1;
	}
	POCKET_GL_ERROR_MSG("glew skip error.");

	// デバッグ機能が使えるか
	std::cout << "GL_ARB_debug_output: " << gl::is_extension_support("GL_ARB_debug_output") << std::endl;

	// 頂点シェーダ―作成
	gl::shader vert = gl::make_vertex_shader("test.vert");
	if (!vert)
	{
		std::cout << vert.error() << std::endl;
	}
	else
	{
		std::cout << vert << std::endl;
	}
	POCKET_GL_ERROR();

	// フラグメントシェーダー作成
	gl::shader frag = gl::make_fragment_shader("test.frag");
	if (!frag)
	{
		std::cout << frag.error() << std::endl;
	}
	else
	{
		std::cout << frag << std::endl;
	}
	POCKET_GL_ERROR();

	// パイプライン管理プログラム作成
	gl::program prog = gl::make_program(vert, frag, true);
	//gl::program prog = gl::make_program("test.shbin", true);
	if (!prog)
	{
		std::cout << prog.error() << std::endl;
	}
	else
	{
		std::cout << prog << std::endl;
		// バイナリの保存
		if (!prog.save_binary("test.shbin", true))
		{
			std::cout << prog.error() << std::endl;
			prog.clear();
		}
		// UBOの情報を出力
		prog.reflect_uniform_block(std::cout);
	}
	POCKET_GL_ERROR();

	// uniform buffer object作成
	ublock data;
	data.world.load_identity();
	data.ortho.load_orthographics2d(0.0f, 640.0f, 0.0f, 480.0f);
	gl::uniform_buffer ubo = prog.make_uniform_buffer("ublock", 0, data);
	if (!ubo)
	{
		std::cout << ubo.error() << std::endl;
	}
	else
	{
		std::cout << ubo << std::endl;
	}
	POCKET_GL_ERROR();

	// レイアウトを指定した頂点バッファを作成
	gl::vertex_layout layouts[] = {
		gl::layout_t<2, float, false, offsetof(simple_vertex, position)>::value,
		gl::layout_t<4, float, false, offsetof(simple_vertex, color)>::value,
	};
	simple_vertex vertices[] = {
		{math::vector2f(320.0f, 0.0f), math::colorf::red},
		{math::vector2f(0.0f, 480.0f), math::colorf::blue},
		{math::vector2f(640.0f, 480.0f), math::colorf::green}
	};
	gl::layered_vertex_buffer<simple_vertex> lvb = gl::make_layered_vertex_buffer<simple_vertex>(vertices, layouts);
	if (!lvb)
	{
		std::cout << lvb.error() << std::endl;
	}
	else
	{
		gl::layered_vertex_buffer<simple_vertex>::binder_type bind = lvb.make_binder();
		std::cout << *bind << std::endl;
	}
	POCKET_GL_ERROR();

	// インダイレクトバッファ作成
	gl::commands::draw_arrays cmd = _ARRAY_SIZE(vertices); // 描画数
	gl::buffer indirect = gl::make_draw_indirect_buffer_immutable(cmd);
	if (!indirect)
	{
		std::cout << indirect.error() << std::endl;
	}
	else
	{
		gl::buffer::binder_type bind = indirect.make_binder();
		std::cout << *bind << std::endl;
	}
	POCKET_GL_ERROR();

	// 受け取るメッセージバッファ
	char message[256];

	do
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// UBO更新
		float time = math::math_traitsf::sin(static_cast<float>(glfwGetTime()*120.0f));
		data.world.load_world(math::vector3f::one, math::quaternionf::identity, math::vector3f(time*30.0f, 0.0f, 0.0f));
		ubo.uniform(0, data.world); // オフセットを指定して指定した型サイズのみ更新
		//ubo.uniform(data); // 全体の更新

		// プログラムバインド
		prog.bind();
		// UBOバインド
		ubo.bind();
		// 頂点バッファバインド
		lvb.bind();
		// インダイレクトバッファバインド
		indirect.bind();

		// バインドされている状態で描画できる状態か確認
		if (!prog.drawable(message))
		{
			// 描画できる状態じゃなければ終了
			std::cout << message << std::endl;
			break;
		}

		// 描画
		//POCKET_GL_FUNC(glDrawArrays, GL_TRIANGLES, 0, lvb.count());
		POCKET_GL_FUNC(glDrawArraysIndirect, GL_TRIANGLES, NULL);

		// バインド解除
		prog.unbind();
		lvb.unbind();
		indirect.unbind();

		glfwSwapBuffers(window);
	} while ((glfwGetKey(window, GLFW_KEY_ESCAPE) | glfwWindowShouldClose(window)) == GL_FALSE);

	indirect.finalize();
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
