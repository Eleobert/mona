#pragma once

#include <string>
#include <glm/glm.hpp>


class shader
{
    unsigned int program;

public:
    shader(const std::string& vshader_filename, const std::string& fshader_filename,
           const std::string& gshader_filename = "");
    void use();
    void unuse();
    void set_uniform(const std::string& name, const glm::mat4& mat);
    void set_uniform(const std::string& name, int value);
    void set_uniform(const std::string& name, float value);
    void set_uniform(const std::string& name, const glm::vec3& vec);
    void set_uniform(const std::string& name, const glm::vec4& vec);
    void set_uniform(const std::string& name, const glm::ivec2& vec);


};
