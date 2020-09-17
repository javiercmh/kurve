#ifndef MESH_HPP
#define MESH_HPP

#include "gl_calls.hpp"

extern "C" {
	// Include the GLAD loader *before* including GLFW!
	#include "glad/glad.h"
	
	// Include the GLFW library (should be the same for all OS):
	#include <GLFW/glfw3.h>
}

class Mesh {
   protected:
    GLuint vao; // Vertex array object
	GLuint vbo; // Vertex buffer object
	// GLuint ebo; // Element buffer object
	GLuint ubo; // Uniform buffer object
    int id;
    void bind() {
        glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    }
   public:
    int get_id() {
        return this->id;
    }
    void draw() {};
    void init();
};

typedef struct
{
	GLfloat position[3];
	GLubyte color[3];
} vertex_data_t;

#endif