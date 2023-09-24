#pragma once

#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <cstddef>

class IndexBuffer {
  public:
    IndexBuffer() = default;
    IndexBuffer(const unsigned int* data, size_t count) noexcept;
    ~IndexBuffer() noexcept;

    void Bind() const noexcept;
    void Unbind() const noexcept;

    size_t Count() const noexcept {
        return count;
    }

    GLuint GLID() const noexcept {
        return glID;
    }

    void Clean() noexcept {
        glID  = 0;
        count = 0;
    }

  private:
    GLuint glID{};
    size_t count{};
};
