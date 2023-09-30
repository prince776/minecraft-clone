#include "game/world.hpp"
#include "ext/vector_float3.hpp"
#include "fmt/core.h"
#include "game/chunk.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "tl/expected.hpp"
#include <cstddef>
#include <sys/_types/_uintptr_t.h>
#include <tuple>

World::World(int l, int w) noexcept {
    for (int x = 0; x < l; x++) {
        for (int z = 0; z < w; z++) {
            chunks.push_back(Chunk(glm::vec3(x * Chunk::BlockCount, 0, z * Chunk::BlockCount)));
        }
    }
}

void World::Tick(World& world) noexcept {
    for (auto& chunk : chunks) {
        chunk.Tick(world);
    }
}

void World::Render(const Renderer& renderer, const Shader& shader) noexcept {
    for (auto& chunk : chunks) {
        chunk.Render(renderer, shader, *this);
    }
    for (auto& chunk : chunks) {
        chunk.RenderWater(renderer, shader, *this);
    }
}

Chunk* World::LookUpChunk(int x, int y, int z) noexcept {
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

std::pair<glm::vec<3, int>, Chunk*> World::BlockAt(glm::vec3 pos) noexcept {
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    int chunkX = (x / Chunk::BlockCount) * Chunk::BlockCount;
    int chunkY = (y / Chunk::BlockCount) * Chunk::BlockCount;
    int chunkZ = (z / Chunk::BlockCount) * Chunk::BlockCount;
    if (x < 0)
        chunkX -= Chunk::BlockCount;
    if (y < 0)
        chunkY -= Chunk::BlockCount;
    if (z < 0)
        chunkZ -= Chunk::BlockCount;

    auto* chunk = LookUpChunk(chunkX, chunkY, chunkZ);
    // fmt::println("Got chunk: {}: at {} {} {}", (uintptr_t)chunk, chunkX, chunkY, chunkZ);
    if (chunk == nullptr) {
        return {glm::vec<3, int>(0), nullptr};
    }

    // auto isBlockPresent = chunk->cubes[x - chunkX][y - chunkY][z - chunkZ].state !=
    // ChunkCube::State::NOT_PRESENT;
    int tx = x - chunkX;
    int ty = y - chunkY;
    int tz = z - chunkZ;
    if (tx < 0)
        tx += 2 * Chunk::BlockCount;
    if (ty < 0)
        ty += 2 * Chunk::BlockCount;
    if (tz < 0)
        tz += 2 * Chunk::BlockCount;

    return {glm::vec<3, int>(tx, ty, tz), chunk};
}
