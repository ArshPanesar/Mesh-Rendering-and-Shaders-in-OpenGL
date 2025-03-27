#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    GLuint program;

public:
    Shader();
    ~Shader();

    void load(const char* vertex_file, const char* frag_file);
    
    inline void use() { glUseProgram(program); }

    inline void set_uniform_mat4(const char* name, const glm::mat4& mat4) {

        GLint location = glGetUniformLocation(program, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
    }

    inline void set_uniform_vec2(const char* name, const glm::vec2& vec) {

        GLint location = glGetUniformLocation(program, name);
        glUniform2f(location, vec.x, vec.y);
    }

    inline void set_uniform_vec3(const char* name, const glm::vec3& vec) {

        GLint location = glGetUniformLocation(program, name);
        glUniform3f(location, vec.x, vec.y, vec.z);
    }

    inline void set_uniform_bool(const char* name, const bool& val) {

        GLint location = glGetUniformLocation(program, name);
        glUniform1i(location, val);
    }
};
