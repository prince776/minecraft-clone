#pragma once

#include "renderer/core.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex-array.hpp"
#include "renderer/vertex-buffer.hpp"

class Renderer {
  public:
    void clear() const noexcept;
    void draw(const VertexArray& vao,
              const IndexBuffer& ibo,
              const Shader& shader) const noexcept;
};
