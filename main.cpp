#include "pocket/math/all.h"
#include "pocket/gl/all.h"
#include <GLFW/glfw3.h>
#include <iomanip>

class Lock
{
public:
	Lock(GLFWwindow* win) :
		win(win)
	{

	}
	~Lock()
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
	Lock lock(window);
	if (window == NULL)
	{
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	err = glewInit();
	if (err != GLEW_NO_ERROR)
	{
		std::cout << reinterpret_cast<const char*>(glewGetErrorString(err)) << std::endl;
		return 1;
	}

	/*std::cout << "version: " << pocket::gl::get_version() << std::endl <<
		"vendor: " << pocket::gl::get_vendor_name() << std::endl <<
		"renderer: " << pocket::gl::get_renderer_name() << std::endl;*/

	glfwSwapInterval(1);

	const char* paths[] = {
		"test_inv.vert",
		"test.vert"
	};
	pocket::gl::shader vert = pocket::gl::make_vertex_shader(paths);
	if (!vert)
	{
		std::cout << vert << std::endl;
	}
	pocket::gl::shader frag = pocket::gl::make_fragment_shader("test.frag");
	if (!frag)
	{
		std::cout << frag << std::endl;
	}
	pocket::gl::program prog = pocket::gl::make_program(vert, frag);
	if (!prog)
	{
		std::cout << prog << std::endl;
	}

	prog[prog["b1"]] = 0;

	float time = 0.0f;

	do
	{
		glfwPollEvents();

		time = pocket::math_traitsf::abs(pocket::math_traitsf::sin(static_cast<float>(glfwGetTime()*60.0f)));
		glClearColor(time, time, time, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	} while ((glfwGetKey(window, GLFW_KEY_ESCAPE) | glfwWindowShouldClose(window)) == GL_FALSE);

	prog.finalize();
	vert.finalize();
	frag.finalize();
}
