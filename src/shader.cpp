#include "shader.hpp"
#include "gl_calls.hpp"

#include <iostream>

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "obj.h"
#include "bitmap.h"
}

Shader::Shader(std::string vs_path, std::string fs_path, user_data_t* user_data) {
    user_data = user_data;

    // Create the vertex shader:
    printf("Compiling vertex shader ...\n");
    GLuint vertex_shader = Shader::compile(
        GL_VERTEX_SHADER, vs_path.c_str(), "Vertex shader");

    // Create the fragment shader:
    printf("Compiling fragment shader ...\n");
    GLuint fragment_shader = Shader::compile(
        GL_FRAGMENT_SHADER, fs_path.c_str(), "Fragment shader");

    // Create an empty shader program:
    printf("Creating shader program ...\n");

    this->id = glCreateProgram();
    gl_check_error("glCreateProgram");
    std::cout << "Shader Program ID: " << this->id << std::endl;

    // Attach both shaders to the program:
    glAttachShader(this->id, vertex_shader);
    gl_check_error("glAttachShader [vertex]");

    glAttachShader(this->id, fragment_shader);
    gl_check_error("glAttachShader [fragment]");

    // Link the shader program:
    glLinkProgram(this->id);
    gl_check_error("glLinkProgram");

    // Detach the shaders after linking:
    glDetachShader(this->id, vertex_shader);
    gl_check_error("glDetachShader [vertex]");

    glDetachShader(this->id, fragment_shader);
    gl_check_error("glDetachShader [fragment]");

    // Delete the shaders:
    glDeleteShader(vertex_shader);
    gl_check_error("glDeleteShader [vertex]");

    glDeleteShader(fragment_shader);
    gl_check_error("glDeleteShader [fragment]");

    // Check the link status:
    GLint success;

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    gl_check_error("glGetProgramiv");

    if (success) {
        // Use the program from now on:
        glUseProgram(this->id);
        gl_check_error("glUseProgram");

        // Store it inside our user data struct:
        user_data->shader_program = this->id;

        // We can now release the shader compiler.
        glReleaseShaderCompiler();
        gl_check_error("glReleaseShaderCompiler");

        return;
    }

    // Extract the length of the error message (including '\0'):
    GLint info_length = 0;
    glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &info_length);
    gl_check_error("glGetProgramiv");

    if (info_length > 1) {
        // Extract the message itself:
        char* info = (char*)malloc(info_length);
        check_error(info != NULL, "Failed to allocate memory for error message.");

        glGetProgramInfoLog(this->id, info_length, NULL, info);
        gl_check_error("glGetProgramInfoLog");

        fprintf(stderr, "Error linking shader program: %s\n", info);
        free(info);
    } else {
        fprintf(stderr, "No info log from the shader compiler :(\n");
    }
}

GLuint Shader::get_id() {
	return this->id;
}

char* Shader::read_from_file(const char* path) {
    // Open the file:
    FILE* file = fopen(path, "rb");
    check_error(file != NULL, "Failed to open file.");

    // Seek to the end:
    int success = fseek(file, 0, SEEK_END);
    check_error(success == 0, "Failed to forward file pointer.");

    // Obtain the size of the file from the position of the file pointer:
    long size = ftell(file);
    check_error(size >= 0, "Failed to determine file size.");

    // Rewind the file pointer back to the start:
    rewind(file);

    // Allocate the output buffer:
    char* buffer = (char*)malloc(size + 1);
    check_error(buffer != NULL, "Failed to allocate buffer.");

    // Read into it:
    size_t blocks_read = fread(buffer, size, 1, file);
    check_error(blocks_read == 1, "Failed to read file.");

    // Close the file:
    success = fclose(file);
    check_error(success == 0, "Failed to close file.");

    // Append a null-terminator and return the buffer:
    buffer[size] = '\0';

    return buffer;
}

GLuint Shader::compile(
    GLenum type, const char* shader_path, const char* shader_tag) {
    // Create an empty shader:
    GLuint shader = glCreateShader(type);
    gl_check_error("glCreateShader");

    // Read and specify the source code:
    char* shader_source = Shader::read_from_file(shader_path);

    glShaderSource(shader, 1, (const char**)&shader_source, NULL);
    gl_check_error("glShaderSource");

    free(shader_source);

    // Compile the shader:
    glCompileShader(shader);
    gl_check_error("glCompileShader");

    // Check the compilation status:
    GLint success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    gl_check_error("glGetShaderiv");

    if (success) {
        return shader;
    }

    // Extract the length of the error message (including '\0'):
    GLint info_length = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
    gl_check_error("glGetShaderiv");

    if (info_length > 1) {
        // Extract the message itself:
        char* info = (char*)malloc(info_length);
        check_error(info != NULL, "Failed to allocate memory for error message.");

        glGetShaderInfoLog(shader, info_length, NULL, info);
        gl_check_error("glGetShaderInfoLog");

        fprintf(stderr, "Error compiling shader (%s): %s\n", shader_tag, info);
        free(info);
    } else {
        fprintf(stderr, "No info log from the shader compiler :(\n");
    }

    exit(EXIT_FAILURE);
}