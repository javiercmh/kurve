#ifndef DISPLAY_HPP
#define DISPLAY_HPP

extern "C" {
	// Include the GLAD loader *before* including GLFW!
	#include "glad/glad.h"
	
	// Include the GLFW library (should be the same for all OS):
	#include <GLFW/glfw3.h>
}
#include <iostream>
#include "gl_calls.hpp"

void error_callback(int error, const char* description);

void framebuffer_size_callback(GLFWwindow* window, int fb_width, int fb_height);

void window_size_callback(GLFWwindow* window, int width, int height);

class Display {
   public:
    GLFWwindow* window;
    user_data_t* user_data;
    Display(user_data_t* user_data);
    GLFWwindow* get_window();
    int terminate();
};

#endif