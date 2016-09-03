﻿#include "pocket/math/all.h"
#include "pocket/gl/all.h"
#include <GLFW/glfw3.h>
#include <iomanip>
#include <vector>
#include <typeinfo>

namespace io = pocket::io;
namespace gl = pocket::gl;

// ウィンドウを見せてメインループを回すか
#define _SHOW_WINDOW 0

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

#define _ARRAY_SIZE(ARY) (sizeof(ARY) / sizeof(ARY[0]))

struct UniformStructure
{
	pocket::vector2f tex;
	pocket::colorf col;
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
#if !_SHOW_WINDOW
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
#endif
	glfwSetErrorCallback(&glfw_error_log);

	GLFWwindow* window = glfwCreateWindow(640, 480, "test", NULL, NULL);
	const main_lock lock(window);
	if (window == NULL)
	{
		return 1;
	}
#if _SHOW_WINDOW
	glfwSetWindowPos(window, 4, 28);
	glfwSwapInterval(1);
#endif
	glfwMakeContextCurrent(window);

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
		if (!prog.save_binary("test.shbin", true))
		{
			std::cout << prog.error() << std::endl;
			prog.clear();
		}
		gl::program::binder_type bind = prog.make_binder();
		std::cout << *bind << std::endl;
		prog.refrect_uniform_block(std::cout, io::tab);
	}
	POCKET_GL_ERROR();

	// バッファ作成
	float ary[] = { 0.0f, 1.0f, 5.0f, 10.0f, 2.0f };
	gl::buffer buffer = gl::make_array_buffer_immutable(ary);
	//gl::commands::draw_arrays command(12);
	//gl::buffer buffer = gl::make_draw_indirect_buffer_immutable(command);
	if (!buffer)
	{
		std::cout << buffer.error() << std::endl;
	}
	else
	{
		gl::buffer::binder_type bind = buffer.make_binder();
		std::cout << *bind << std::endl;
	}
	POCKET_GL_ERROR();

	// uniform buffer object作成
	gl::uniform_buffer uniform = prog.make_uniform_buffer("vert_uniforms", 0, NULL, gl::buffer_usage::dynamic_draw);
	if (!uniform)
	{
		std::cout << uniform.error() << std::endl;
	}
	else
	{
		//gl::uniform_buffer::binder_type bind = uniform.make_binder();
		//std::cout << *bind << std::endl;
		std::cout << uniform << std::endl;
	}
	POCKET_GL_ERROR();

	gl::vertex_layout_index layouts[] = {
		{0, 3, GL_FLOAT, GL_FALSE, 0},
		{2, 4, GL_FLOAT, GL_FALSE, sizeof(float[3])}
	};
	gl::vertex_array vao = gl::make_vertex_array(buffer, sizeof(float[3])+sizeof(float[4]), layouts);
	if (!vao)
	{
		std::cout << vao.error() << std::endl;
	}
	else
	{
		gl::vertex_array::binder_type bind = vao.make_binder();
		std::cout << *bind << std::endl;
	}
	POCKET_GL_ERROR();

#if _SHOW_WINDOW
	// メインループ
	do
	{
		glfwPollEvents();

		float time = pocket::math_traitsf::abs(pocket::math_traitsf::sin(static_cast<float>(glfwGetTime()*60.0f)));
		glClearColor(time, time, time, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	} while ((glfwGetKey(window, GLFW_KEY_ESCAPE) | glfwWindowShouldClose(window)) == GL_FALSE);
#endif

	vao.finalize();
	uniform.finalize();
	buffer.finalize();
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
