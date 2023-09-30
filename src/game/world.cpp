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
            int tx = x * Chunk::BlockCount, tz = z * Chunk::BlockCount;
            std::tuple<int, int, int> key(tx, 0, tz);
            chunks.emplace(key, glm::vec3(tx, 0, tz));
        }
    }
}

void World::Tick(World& world) noexcept {
    for (auto& [_, chunk] : chunks) {
        chunk.Tick(world);
    }
}

void World::Render(const Renderer& renderer, const Shader& shader) noexcept {
    for (auto& [_, chunk] : chunks) {
        chunk.Render(renderer, shader, *this);
    }
    for (auto& [_, chunk] : chunks) {
        chunk.RenderWater(renderer, shader, *this);
    }
}

Chunk* World::LookUpChunk(int x, int y, int z) noexcept {
    auto key = std::tuple<int, int, int>(x, y, z);

    if (!chunks.contains(key)) {
        return nullptr;
    }

    return &chunks.at(key);
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
    // if (y < 0)
    //     chunkY -= Chunk::BlockCount;
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
    // if (ty < 0)
    //     ty += 2 * Chunk::BlockCount;
    if (tz < 0)
        tz += 2 * Chunk::BlockCount;

    if (tx < 0 || ty < 0 || tz < 0 || tx >= Chunk::BlockCount || ty >= Chunk::BlockCount ||
        tz >= Chunk::BlockCount) {
        return {glm::vec<3, int>(0), nullptr};
    }
    return {glm::vec<3, int>(tx, ty, tz), chunk};
}
