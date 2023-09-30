#pragma once

#include "game/chunk.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include <map>
#include <tuple>
#include <utility>
#include <vector>

#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "tl/expected.hpp"
#include <array>
#include <valarray>
#include <vector>

class World {
  public:
    World(int l, int w) noexcept;

    void Tick(World& world) noexcept;
    void Render(const Renderer& renderer, const Shader& shader) noexcept;

    Chunk* LookUpChunk(int x, int y, int z) noexcept;

    std::pair<glm::vec<3, int>, Chunk*> BlockAt(glm::vec3 pos) noexcept;

  private:
    std::vector<Chunk> chunks;
    std::map<std::tuple<int, int, int>, size_t> chunkLookUpTable;
};
