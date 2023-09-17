#pragma once

#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <cstddef>

class IndexBuffer {
  public:
    IndexBuffer(const unsigned int* data, size_t count) noexcept;
    ~IndexBuffer() noexcept;

    void Bind() const noexcept;
    void Unbind() const noexcept;

    size_t Count() const noexcept {
        return count;
    }

  private:
    GLuint glID;
    size_t count;
};
