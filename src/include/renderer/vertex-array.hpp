#pragma once

#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <OpenGL/gltypes.h>

class VertexArray {
  public:
    VertexArray() noexcept;
    ~VertexArray() noexcept;

    void Bind() const noexcept;
    void Unbind() const noexcept;
    void AddBuffer(const VertexBuffer& vbo,
                   const VertexBufferLayout& layout) noexcept;

  private:
    GLuint glID;
};
