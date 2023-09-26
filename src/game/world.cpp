#include "game/world.hpp"
#include "ext/vector_float3.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "tl/expected.hpp"
#include <tuple>

World::World(int l, int w) noexcept {
    for (int x = 0; x < l; x++) {
        for (int z = 0; z < w; z++) {
            chunks.push_back(Chunk(glm::vec3(x * Chunk::BlockCount, 0, z * Chunk::BlockCount)));
        }
    }
}

void World::Tick() noexcept {
}

void World::Render(const Renderer& renderer, const Shader& shader) noexcept {
    for (auto& chunk : chunks) {
        chunk.Render(renderer, shader, *this);
    }
}

const Chunk* World::LookUpChunk(int x, int y, int z) noexcept {
    auto key = std::tuple<int, int, int>(x, y, z);

    if (!chunkLookUpTable.contains(key)) {
        auto found = -1;
        for (size_t idx = 0; idx < chunks.size(); idx++) {
            auto chunkPos = chunks[idx].Pos();
            if (chunkPos.x == x && chunkPos.y == y && chunkPos.z == z) {
                found = idx;
                break;
            }
        }

        if (found == -1) {
            return nullptr;
        }

        chunkLookUpTable[key] = found;
    }

    auto idx = chunkLookUpTable[key];
    return &chunks[idx];
}
