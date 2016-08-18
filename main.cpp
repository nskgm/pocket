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
			prog.clear();
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

	//float ary[] = { 0.0f, 10.0f, 20.025f, 3.125f, 400.01f, 10.0f };
	gl::commands::draw_arrays cmd(3, 1, 0);
	// データからバッファを作成
	// -- make_buffer
	// -- make_[type]_buffer
	// -- make_[type]_buffer_[usage(immutable, dynamic, stream)]
	// make_bufferの時はtypeとusageを指定
	// make_[type]_bufferの時はusageを指定
	// -- type -> pocket::gl::buffer_base::buffer_type
	// -- usage -> pocket::gl::buffer_base::usage_type
	//gl::buffer buf = gl::make_array_buffer_immutable(ary);
	gl::buffer buf = gl::make_draw_indirect_buffer_immutable(cmd);
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
		gl::buffer::binder_type bind = buf.make_binder();

		// binder_typeで受け取っている時のsizeやcountは自動でバインドされているものが使用される
		std::cout << bind.size() << std::endl;
		std::cout << bind.usage() << std::endl;

		// CPU上に展開してGPU上の情報を得る
		// 自動で展開の解除
		//bind->map_binding<int>(gl::buffer_base::read, [&bind](int* a) {
		//	for (int i = 0; i < bind->count_binding<int>(); ++i)
		//	{
		//		std::cout << a[i] << " ";
		//	}
		//	std::cout << std::endl;
		//});

		gl::buffer::rebinder_map<gl::commands::draw_arrays>::type map = bind.make_binder_map<gl::commands::draw_arrays>(gl::buffer::read);
		if (map)
		{
			//int n = bind.count<float>();
			//for (int i = 0; i < n; ++i, ++map)
			//{
			//	//std::cout << map[i] << " ";
			//	std::cout << *map << " ";
			//}
			//std::cout << std::endl;
			std::cout << map->arrays << " " << map->instance << " " << map->index << std::endl;
		}
	}

	{
		gl::program::binder_type bind = prog.make_binder();
		std::cout << bind->binding() << std::endl;
	}
	std::cout << prog.binding() << std::endl;

	// バインドされていない状態でのサイズ取得
	// 必ずバインドしないといけないので内部で行っている
	// サイズが取得できたらバインドも解除されてしまう
	// std::cout << buf.size() << std::endl;

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

#ifdef _MSC_VER
#pragma comment (lib, "OpenGL32.lib")
#ifdef _DEBUG
#ifdef _MT
#ifdef _DLL
#pragma comment (lib, "glfw3_mdd.lib")
#pragma comment (lib, "glew32s_mdd.lib")
#else
#pragma comment (lib, "glfw3_mtd.lib")
#pragma comment (lib, "glew32s_mtd.lib")
#endif
#else
#error No MultiThread.
#endif
#else
#ifdef _MT
#ifdef _DLL
#pragma comment (lib, "glfw3_md.lib")
#pragma comment (lib, "glew32s_md.lib")
#else
#pragma comment (lib, "glfw3_mt.lib")
#pragma comment (lib, "glew32s_mt.lib")
#endif
#else
#error No MultiThread.
#endif
#endif /* _MT */
#endif // _MSC_VER
