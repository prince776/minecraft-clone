#pragma once

#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <string>

struct ShaderProgramSource {
    std::string vertexSource, fragmentSource;
};

class Shader {
  public:
    Shader(const std::string& filepath) noexcept;
    ~Shader();

    void Bind() const noexcept;
    void Unbind() const noexcept;

  private:
    std::string filepath;
    GLuint glID;

  private:
    ShaderProgramSource parseShaderFile(const std::string& path) const noexcept;
    GLuint
    createShaderProgram(const std::string& vertexShader,
                        const std::string& fragmentShader) const noexcept;
    GLuint compileShader(GLenum type, const std::string& src) const noexcept;
};
