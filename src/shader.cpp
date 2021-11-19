

#include "mona/internal/shader.hpp"
#include "slam/slam.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

auto compile(gl::GLenum shader_type, const std::string& path) -> gl::GLuint
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::string source( (std::istreambuf_iterator<char>(file)) , std::istreambuf_iterator<char>());

    auto id = gl::glCreateShader(shader_type);

    const char *c_str = source.c_str();
    gl::glShaderSource(id, 1, &c_str, nullptr);
    gl::glCompileShader(id);

    int success = gl::GL_FALSE;
    gl::glGetShaderiv(id, gl::GL_COMPILE_STATUS, &success);

    if(success != gl::GL_TRUE)
    {
        char errorString[512];
        gl::glGetShaderInfoLog(id, sizeof(errorString), nullptr, errorString);
        std::clog << errorString << " " << path;
    }

    return id;
}

auto link(gl::GLuint vshader, gl::GLuint fshader, gl::GLuint gshader) -> gl::GLuint
{
    auto program = gl::glCreateProgram();
    gl::glAttachShader(program, vshader);
    gl::glAttachShader(program, fshader);
    if(gshader) gl::glAttachShader(program, gshader);

    gl::glLinkProgram(program);

    auto success = 0;

    gl::glGetProgramiv(program, gl::GL_LINK_STATUS, &success);

    if(success != gl::GL_TRUE)
    {
        char msg[512];
        gl::glGetProgramInfoLog(program, sizeof(msg), nullptr, msg);
        std::clog << msg << "\n";
    }

    return program;
}

shader::shader(const std::string& vertexShaderSourcePath,
               const std::string& fragmentShaderSourcePath,
               const std::string& geom_shader_filename)
{
    auto vshader = compile(gl::GL_VERTEX_SHADER, vertexShaderSourcePath);
    auto fshader = compile(gl::GL_FRAGMENT_SHADER, fragmentShaderSourcePath);
    auto gshader = (geom_shader_filename.empty())? 0: compile(gl::GL_GEOMETRY_SHADER,
                                                              geom_shader_filename);

    program = link(vshader, fshader, gshader);

    gl::glDeleteShader(vshader);
    gl::glDeleteShader(fshader);
    gl::glDeleteShader(gshader);
}


void shader::use()
{
    gl::glUseProgram(program);
}

void shader::unuse()
{
    gl::glUseProgram(0);
}



void shader::set_uniform(const std::string& name, const glm::mat4& mat)
{
    this->use();
    auto loc = gl::glGetUniformLocation(program, name.c_str());
    gl::glUniformMatrix4fv(loc, 1, gl::GL_FALSE, glm::value_ptr(mat));
}

void shader::set_uniform(const std::string& name, int value)
{
    this->use();
    int loc = gl::glGetUniformLocation(program, name.c_str());
    gl::glUniform1i(loc, value);
}

void shader::set_uniform(const std::string& name, float value)
{
    this->use();
    int loc = gl::glGetUniformLocation(program, name.c_str());
    gl::glUniform1f(loc, value);
}


void shader::set_uniform(const std::string& name, const glm::vec3& vec)
{
    this->use();
    int loc = gl::glGetUniformLocation(program, name.c_str());
    gl::glUniform3f(loc, vec.x, vec.y, vec.z);
}

void shader::set_uniform(const std::string& name, const glm::vec4& vec)
{
    this->use();
    int loc = gl::glGetUniformLocation(program, name.c_str());
    gl::glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}


void shader::set_uniform(const std::string& name, const glm::ivec2& vec)
{
    this->use();
    int loc = gl::glGetUniformLocation(program, name.c_str());
    gl::glUniform2i(loc, vec.x, vec.y);
}
