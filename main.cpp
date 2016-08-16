#include "pocket/math/all.h"
#include "pocket/gl/all.h"
#include <GLFW/glfw3.h>
#include <iomanip>

namespace gl = pocket::gl;

class main_lock
{
public:
	main_lock(GLFWwindow* win) :
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

int main()
{
	std::cout << std::boolalpha << std::fixed;

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

	glfwSetErrorCallback(&glfw_error_log);

	GLFWwindow* window = glfwCreateWindow(640, 480, "test", NULL, NULL);

	main_lock lock(window);
	if (window == NULL)
	{
		return 1;
	}
	glfwSetWindowPos(window, 4, 28);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	err = glewInit();
	if (err != GLEW_NO_ERROR)
	{
		std::cout << reinterpret_cast<const char*>(glewGetErrorString(err)) << std::endl;
		return 1;
	}

	// 頂点シェーダ―の作成
	// -- make_shader
	// -- make_[type]_shader
	// make_shaderの時はtypeを第一引数で指定
	// type -> pocket::gl::shader::shader_type
	// 最後の引数でpocket::gl::shader::memoryを指定で文字列から作成
	// デフォルト: pocket::gl::shader::file (ファイルパスから作成)
	gl::shader vert = gl::make_vertex_shader("test.vert");
	// 作成できているか
	if (!vert)
	{
		//	shader: {
		//		type: [type],			// シェーダーの種類
		//		valid: (true, false)	// 有効な状態か
		//	----- エラー発生していたら
		//		error: *****			// エラーの内容
		//	-----
		//	}
		std::cout << vert << std::endl;
	}
	// フラグメントシェーダ―作成
	// 上記同様
	gl::shader frag = gl::make_fragment_shader("test.frag");
	if (!frag)
	{
		std::cout << frag << std::endl;
	}
	// シェーダーからプログラムを作成
	// 最大シェーダー引数は5
	// 最後の真偽はバイナリの保存するか
	// デフォルト: false
	gl::program prog = gl::make_program(vert, frag, true);

	// 保存されているバイナリからプログラムを作成
	// 最後の真偽はバイナリファイルの最初にバイナリファイルのフォーマットが書き込まれているか
	// デフォルト: true
	//gl::program prog = gl::make_program("test.shbin", true);

	if (prog)
	{
		// シェーダーから作成時の最後の引数をtrueにした場合に保存できる
		// 最後の真偽はバイナリファイルの最初にバイナリファイルのフォーマットを書き込むか
		// デフォルト: true
		if (!prog.save_binary("test.shbin", true))
		{
			std::cout << prog << std::endl;
		}
	}
	else
	{
		//	program: {
		//		valid: (true, false)	// 有効な状態か
		//	----- エラー発生していたら
		//		error: *****			// エラーの内容
		//	-----
		//	}
		std::cout << prog << std::endl;
	}

	int ary[] = { 0, 1, 2, 3, 4 };
	// データからバッファを作成
	// -- make_buffer
	// -- make_[type]_buffer
	// -- make_[type]_buffer_[usage(immutable, dynamic, stream)]
	// make_bufferの時はtypeとusageを指定
	// make_[type]_bufferの時はusageを指定
	// -- type -> pocket::gl::buffer_base::buffer_type
	// -- usage -> pocket::gl::buffer_base::usage_type
	gl::buffer buf = gl::make_array_buffer_immutable(ary);
	if (!buf)
	{
		//	buffer: {
		//		type: [type],			// バッファの種類
		//		valid: (true, false)	// 有効な状態か
		//	----- エラー発生していたら
		//		error: *****			// エラーの内容
		//	-----
		//	}
		std::cout << buf << std::endl;
	}
	{
		// コンストラクタでバインドをしてデストラクタでバインドを解除するクラスを作成
		// pocket::gl::binder -> 引数なし
		// pocket::gl::binder1 -> 引数１
		gl::binder1<gl::buffer, GLenum> bind = gl::make_binder<GL_ARRAY_BUFFER>(buf);
		// operator -> で第一テンプレート型の関数を使用できる
		std::cout << bind->size_binding() << std::endl <<
			bind->usage_binding() << std::endl;

		//std::cout << *bind << std::endl;
	}

	std::cout << pocket::type_traits::extent_count<char>::value << std::endl <<
		pocket::type_traits::extent_count<int[6]>::value << std::endl <<
		pocket::type_traits::extent_count<const float[3]>::value << std::endl;

	// Uniformロケーション取得
	int location = prog["a1"];
	// TODO: 取得したロケーションに対して代入
	prog[location] = 0;

	do
	{
		glfwPollEvents();

		float time = pocket::math_traitsf::abs(pocket::math_traitsf::sin(static_cast<float>(glfwGetTime()*60.0f)));
		glClearColor(time, time, time, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	} while ((glfwGetKey(window, GLFW_KEY_ESCAPE) | glfwWindowShouldClose(window)) == GL_FALSE);

	buf.finalize();
	prog.finalize();
	vert.finalize();
	frag.finalize();
}
