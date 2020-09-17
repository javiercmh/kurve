#include "player_mesh.hpp"

#include <iostream>

extern "C" {
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "obj.h"
}

PlayerMesh::PlayerMesh(user_data_t* user_data) {
    // Triangle data:
    std::vector<vertex_data_t> vertex_data;

    int parts{200};
    double radius{1};
    double delta_angle{(2 * 3.14159265) / parts};
    double angle{0};

    for (int i = 0; i < parts; i++) {
        double x{radius * cos(angle)};
        double y{radius * sin(angle)};
        angle += delta_angle;
        vertex_data.push_back(
            {.position = {static_cast<float>(x), static_cast<float>(y), 0}, .color = {0xFF, 0x00, 0xFF}});
    }

    std::cout << "Vertex in circle: " << vertex_data.size() << std::endl;

    user_data->vertex_data_count += 4;

    // TODO: blackbox! Create a VAO.
    glGenVertexArrays(1, &this->vao);
    gl_check_error("glGenVertexArrays");

    glBindVertexArray(this->vao);
    gl_check_error("glBindVertexArray");

    std::cout << "VAO: " << this->vao << std::endl;

    // Generate and bind a vertex buffer object:
    glGenBuffers(1, &this->vbo);
    gl_check_error("glGenBuffers");

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    gl_check_error("glBindBuffer");

    // Upload the vertex data to the GPU:
    glBufferData(GL_ARRAY_BUFFER, parts * sizeof(vertex_data_t), (const GLvoid*)vertex_data.data(), GL_STATIC_DRAW);
    gl_check_error("glBufferData");

    // Position attribute:
    // Number of attribute, number of components, data type, normalize, stride, pointer (= offset)
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_data_t), (GLvoid*)offsetof(vertex_data_t, position));
    gl_check_error("glVertexAttribPointer [position]");

    glVertexAttribPointer(ATTRIB_COLOR, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex_data_t), (GLvoid*)offsetof(vertex_data_t, color));
    gl_check_error("glVertexAttribPointer [color]");

    // Enable the attributes:
    glEnableVertexAttribArray(ATTRIB_POSITION);
    gl_check_error("glEnableVertexAttribArray [position]");

    glEnableVertexAttribArray(ATTRIB_COLOR);
    gl_check_error("glEnableVertexAttribArray [color]");

    // Generate and bind a uniform buffer object:
    GLuint block_index = glGetUniformBlockIndex(
        user_data->shader_program, "triangleBlock");
    glGenBuffers(1, &this->ubo);
    GLfloat zoom = 1.0;
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat), &zoom, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Store the VBO inside our user data:
    gl_obj_t meta_obj = {
        this->vao,
        this->vbo,
        // ebo,
        this->ubo};
    user_data->vec_obj.push_back(meta_obj);
}