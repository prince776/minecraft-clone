#pragma once
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include <map>
#include <tuple>
#include <vector>

#include "ext/vector_float3.hpp"
#include "game/textute-atlas.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex-array.hpp"
#include <array>
#include <valarray>
#include <vector>

struct ChunkCube {
    enum class State {
        NOT_PRESENT,
        PRESENT,
    };
    State state;
    TexCoord topTile;
    TexCoord bottomTile;
    TexCoord sideTile;
    float alpha{1.0f};

    bool IsTransparent() const noexcept {
        return std::abs(alpha - 1.0f) > 1e-3;
    }
};

using vi   = std::vector<ChunkCube>;
using vii  = std::vector<vi>;
using viii = std::vector<vii>;

class World;

class Chunk {
  public:
    Chunk(const glm::vec3& pos) noexcept;

    void Render(const Renderer& renderer, const Shader& shader, World& world) noexcept;

    void GenerateMesh(World& world) noexcept;

    glm::vec<3, int> Pos() const noexcept {
        return glm::vec<3, int>(pos.x, pos.y, pos.z);
    }

  private:
    glm::vec3 pos;

  public:
    static int constexpr BlockCount = 16; // 16 x 16 x 16
    static int constexpr BlockSize  = 1;

    bool generateMesh{};

    viii cubes;
    VertexArray vao, waterVAO;
    VertexBuffer vbo, waterVBO;
    IndexBuffer ibo, waterIBO;

    TextureAtlas tilesetAtlas{16, 16};
    TexCoord dirtTile{2, 15}, grassTile{0, 15}, grassSideTile{3, 15};
    TexCoord stoneTile{1, 15}, sandTile{0, 4}, waterTile{14, 15};
};
