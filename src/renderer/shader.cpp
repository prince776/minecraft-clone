#include "renderer/shader.hpp"
#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const std::string& filepath) noexcept : filepath(filepath) {
    auto [vertexSource, fragmentSource] = parseShaderFile(filepath);
    glID = createShaderProgram(vertexSource, fragmentSource);
}

Shader::~Shader() {
    GLCALL(glDeleteProgram(glID));
}

void Shader::Bind() const noexcept {
    GLCALL(glUseProgram(glID));
}

void Shader::Unbind() const noexcept {
    GLCALL(glUseProgram(0));
}

void Shader::SetUniformMat4f(const std::string& name,
                             const glm::mat4& mat) noexcept {
    GLCALL(
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniform4f(const std::string& name,
                          float v0,
                          float v1,
                          float v2,
                          float v3) noexcept {
    GLCALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value) noexcept {
    GLCALL(glUniform1i(getUniformLocation(name), value));
}

int Shader::getUniformLocation(const std::string& name) noexcept {
    if (uniformLocCache.contains(name)) {
        return uniformLocCache[name];
    }

    GLCALL(int location = glGetUniformLocation(glID, name.c_str()));
    if (location == -1) {
        std::cout << "[Warning]: "
                  << "Uniform '" << name << "' doesn't exist\n";
    }
    uniformLocCache[name] = location;
    return location;
}

ShaderProgramSource
Shader::parseShaderFile(const std::string& path) const noexcept {
    std::ifstream stream(path);

    enum class ShaderType {
        NONE = -1,
        VERTEX,
        FRAGMENT
    };

    std::string line;
    std::array<std::stringstream, 2> ss;
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << "\n";
        }
    }
    return {
        ss[0].str(),
        ss[1].str(),
    };
}

GLuint
Shader::createShaderProgram(const std::string& vertexShader,
                            const std::string& fragmentShader) const noexcept {
    auto program = glCreateProgram();
    auto vs      = compileShader(GL_VERTEX_SHADER, vertexShader);
    auto fs      = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

GLuint Shader::compileShader(GLenum type,
                             const std::string& src) const noexcept {
    auto srcCStr = src.c_str();
    auto id      = glCreateShader(type);

    glShaderSource(id, 1, &srcCStr, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "["
                  << (type == GL_VERTEX_SHADER ? "Vertex Shader"
                                               : "Fragment Shader")
                  << "]:";
        std::cout << "Failed to compile!\n";
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
