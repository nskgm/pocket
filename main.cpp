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

	gl::shader vert = gl::make_vertex_shader("test.vert");
	if (!vert)
	{
		std::cout << vert << std::endl;
	}
	gl::shader frag = gl::make_fragment_shader("test.frag");
	if (!frag)
	{
		std::cout << frag << std::endl;
	}
	gl::program prog = gl::make_program(vert, frag, true);
	//gl::program prog = gl::make_program("test.shbin", true);
	if (prog)
	{
		if (!prog.save_binary("test.shbin", true))
		{
			std::cout << prog << std::endl;
		}
	}
	else
	{
		std::cout << prog << std::endl;
	}

	int ary[] = {
		0, 1, 2, 3, 4
	};
	gl::buffer buf = gl::make_array_buffer_immutable(ary);
	if (!buf)
	{
		std::cout << buf << std::endl;
	}
	{
		gl::binder1<gl::buffer, GLenum> bind = gl::make_binder<GL_ARRAY_BUFFER>(buf);
		std::cout << bind->size_binding() << std::endl <<
			bind->usage_binding() << std::endl;
	}

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
