#pragma once

#include "renderer/core.hpp"

class VertexBuffer {
  public:
    VertexBuffer(const void* data, size_t size) noexcept;
    ~VertexBuffer();

    void Bind() const noexcept;
    void Unbind() const noexcept;

  private:
    GLuint glID;
};
