#include "line_mesh.hpp"

#include <iostream>
#include <cmath>

void LineMesh::update() {
    this->bind();
    std::vector<vertex_data_t> vertex_data;

    for (auto point : this->points) {        
        vertex_data.push_back(
            {
                .position = {
                    static_cast<GLfloat>(point.x), 
                    static_cast<GLfloat>(point.y), 
                    0
                }, 
                .color = {this->color[0], this->color[1], this->color[2]}
            }
        );
    }

    // Upload the vertex data to the GPU:
    glBufferData(
        GL_ARRAY_BUFFER, 
        this->points.size() * sizeof(vertex_data_t), 
        (const GLvoid*)vertex_data.data(), 
        GL_DYNAMIC_DRAW);
    gl_check_error("glBufferData");

}

void LineMesh::set_points(std::vector<Point> points) {
    this->points = points;
}

void LineMesh::add_point(Point point) {
    // Create left point
    GLfloat numerator{point.y - this->last_point.y};
    GLfloat denominator{point.x - this->last_point.x};
    GLfloat slope{0};
    GLfloat r{0.5};

    Point left_point, right_point;

    if (denominator != 0) {
        slope = numerator / denominator;
        left_point = {
            point.x + -1 * (GLfloat)sin(slope) * r,
            point.y + (GLfloat)cos(slope) * r
        };
        right_point = {
            point.x + (GLfloat)sin(slope) * r,
            point.y + -1 * (GLfloat)cos(slope) * r
        };
    } else {
        left_point = {
            point.x -r,
            point.y
        };
        right_point = {
            point.x + r,
            point.y
        };
    }
    
    this->last_point = point;
    this->points.push_back(left_point);
    this->points.push_back(right_point);
}

LineMesh::LineMesh(Point first_point, std::array<GLubyte, 3> color) {
    this->color = color;
    
    std::vector<vertex_data_t> vertex_data;
    vertex_data.push_back(
        {
            .position = {
                static_cast<GLfloat>(first_point.x), 
                static_cast<GLfloat>(first_point.y), 
                0
            }, 
            .color = {this->color[0], this->color[1], this->color[2]}
        }
    );

    // Create a VAO
    GLuint vao;

    glGenVertexArrays(1, &vao);
    gl_check_error("glGenVertexArrays");

    glBindVertexArray(vao);
    gl_check_error("glBindVertexArray");

    this->vao = vao;
    std::cout << "VAO: " << vao << std::endl;

    // Generate and bind a vertex buffer object (VBO):
    GLuint vbo;

    glGenBuffers(1, &vbo);
    gl_check_error("glGenBuffers");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gl_check_error("glBindBuffer");
    
    this->vbo = vbo;
    std::cout << "VBO: " << vbo << std::endl;

    // Upload the vertex data to the GPU:
    glBufferData(
        GL_ARRAY_BUFFER, 
        this->points.size() * sizeof(vertex_data_t), 
        (const GLvoid*)vertex_data.data(), 
        GL_STATIC_DRAW);
    gl_check_error("glBufferData");

    // Position attribute:
    // Number of attribute, number of components, 
    // data type, normalize, stride, pointer (= offset)
    glVertexAttribPointer(
        ATTRIB_POSITION, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(vertex_data_t), 
        (GLvoid*)offsetof(vertex_data_t, position));
    gl_check_error("glVertexAttribPointer [position]");

    glVertexAttribPointer(
        ATTRIB_COLOR, 
        3, 
        GL_UNSIGNED_BYTE, 
        GL_TRUE, 
        sizeof(vertex_data_t), 
        (GLvoid*)offsetof(vertex_data_t, color));
    gl_check_error("glVertexAttribPointer [color]");

    // Enable the attributes:
    glEnableVertexAttribArray(ATTRIB_POSITION);
    gl_check_error("glEnableVertexAttribArray [position]");

    glEnableVertexAttribArray(ATTRIB_COLOR);
    gl_check_error("glEnableVertexAttribArray [color]");

}

void LineMesh::draw() {
    this->bind();
    glDrawArrays(GL_LINE_STRIP, 0, this->points.size());
    gl_check_error("glDrawArrays");
}