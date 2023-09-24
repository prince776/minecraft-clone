#pragma once

#include "renderer/core.hpp"

class VertexBuffer {
  public:
    VertexBuffer() = default;
    VertexBuffer(const void* data, size_t size) noexcept;
    ~VertexBuffer();

    void Bind() const noexcept;
    void Unbind() const noexcept;

    GLuint GLID() const noexcept {
        return glID;
    }

    void Clean() noexcept {
        glID = 0;
    }

  private:
    GLuint glID{};
};
