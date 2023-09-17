#pragma once

#include "glm.hpp"
#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string vertexSource, fragmentSource;
};

class Shader {
  public:
    Shader(const std::string& filepath) noexcept;
    ~Shader();

    void Bind() const noexcept;
    void Unbind() const noexcept;

    void SetUniformMat4f(const std::string& name,
                         const glm::mat4& mat) noexcept;

    void SetUniform4f(const std::string& name,
                      float v0,
                      float v1,
                      float v2,
                      float v3) noexcept;
    void SetUniform1i(const std::string& name, int v) noexcept;

  private:
    std::string filepath;
    GLuint glID;

    std::unordered_map<std::string, int> uniformLocCache;

  private:
    int getUniformLocation(const std::string& name) noexcept;

    ShaderProgramSource parseShaderFile(const std::string& path) const noexcept;
    GLuint
    createShaderProgram(const std::string& vertexShader,
                        const std::string& fragmentShader) const noexcept;
    GLuint compileShader(GLenum type, const std::string& src) const noexcept;
};
