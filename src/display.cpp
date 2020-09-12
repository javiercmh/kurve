#include "display.hpp"
#include "gl_calls.hpp"
#include <iostream>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s (%d)\n", description, error);
	exit(EXIT_FAILURE);
}

void framebuffer_size_callback(GLFWwindow* window, int fb_width, int fb_height)
{
	glViewport(0, 0, fb_width, fb_height);
	gl_check_error("glViewport");
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	user_data_t* user_data = (user_data_t*) glfwGetWindowUserPointer(window);

	user_data->window_width = width;
	user_data->window_height = height;
}

Display::Display(user_data_t* user_data) {
	this->user_data = user_data;
	printf("Hello triangle!\n");

	// Specify our error callback func:
	// Then initialize GLFW itself.
	printf("Initializing GLFW ...\n");

	glfwSetErrorCallback(error_callback);
	check_error(glfwInit(), "Failed to initialize GLFW.");

	// We want at least OpenGL 4.1:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Enable forward-compatibility and use the core profile:
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window:
	printf("Creating window ...\n");

	this->window = glfwCreateWindow(
        this->user_data->window_width, 
        this->user_data->window_height, 
        "Hello triangle", 
        NULL, 
        NULL);
	check_error(this->window != NULL, "Failed to create window.");

	// Make the OpenGL context of the window the current one:
	glfwMakeContextCurrent(this->window);

	// Loader function:
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Try to swap on every screen update:
	glfwSwapInterval(1);

	// Specify remaining callbacks:
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
	glfwSetWindowSizeCallback(this->window, window_size_callback);

	// Store a pointer to our user data inside the window:
	glfwSetWindowUserPointer(this->window, this->user_data);

	// Initialize everything related to OpenGL:
	init_gl(this->window);
}

GLFWwindow* Display::get_window() {
	return this->window;
}

int Display::terminate() {
	teardown_gl(this->window);

	// Destroy the window:
	glfwDestroyWindow(this->window);

	// Terminate GLFW:
	glfwTerminate();
	return 0;
}