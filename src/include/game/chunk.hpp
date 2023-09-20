#pragma once

#include "ext/vector_float3.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
class Chunk {
  public:
    Chunk(const glm::vec3& pos) noexcept;

    void Render(const Renderer& renderer, const Shader& shader) const noexcept;

  private:
    glm::vec3 pos;

  public:
    static int constexpr BlockCount = 16; // 16 x 16 x 16
    static int constexpr BlockSize  = 1;
};
