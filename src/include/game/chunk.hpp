#pragma once

#include "ext/vector_float3.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include <array>
#include <vector>

struct ChunkCube {
    enum class State {
        NOT_PRESENT,
        PRESENT,
    };
    State state;
};

using vi   = std::vector<ChunkCube>;
using vii  = std::vector<vi>;
using viii = std::vector<vii>;

class Chunk {
  public:
    Chunk(const glm::vec3& pos) noexcept;

    void Render(const Renderer& renderer, const Shader& shader) const noexcept;

  private:
    glm::vec3 pos;

  public:
    static int constexpr BlockCount = 16; // 16 x 16 x 16
    static int constexpr BlockSize  = 1;

    viii cubes;
};
