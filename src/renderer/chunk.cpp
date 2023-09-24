#include "game/chunk.hpp"
#include "fmt/core.h"
#include "game/cube.hpp"
#include "game/textute-atlas.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex-array.hpp"
#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <algorithm>
#include <array>
#include <vector>

Chunk::Chunk(const glm::vec3& pos) noexcept : pos(pos) {
    cubes = viii(BlockCount, vii(BlockCount, vi(BlockCount)));
    for (int x = 0; x < BlockCount; x++) {
        for (int y = 0; y < BlockCount; y++) {
            for (int z = 0; z < BlockCount; z++) {
                cubes[x][y][z] = ChunkCube{
                    .state = ChunkCube::State::PRESENT,
                };
            }
        }
    }
}

// This ordering is coupled with the ordering we get from Cube::Vertices().
static int delX[] = {0, 0, 0, 0, -1, 1};
static int delY[] = {1, -1, 0, 0, 0, 0};
static int delZ[] = {0, 0, 1, -1, 0, 0};

void Chunk::Render(const Renderer& renderer, const Shader& shader) const noexcept {
    std::vector<Vertex> mesh;
    std::vector<unsigned int> indices;
    int prevIndex = 0;

    TextureAtlas tilesetAtlas(16, 16);
    TexCoord dirtTile{2, 15}, grassTile{0, 15}, grassSideTile{3, 15};

    for (int x = 0; x < BlockCount; x++) {
        for (int z = 0; z < BlockCount; z++) {
            for (int y = 0; y < BlockCount; y++) {

                std::array<ChunkCube, 6> adjCubes;

                for (int del = 0; del < 6; del++) {
                    int x1 = x + delX[del];
                    int y1 = y + delY[del];
                    int z1 = z + delZ[del];

                    if (std::min({x1, y1, z1}) < 0 || std::max({x1, y1, z1}) >= BlockCount) {
                        adjCubes[del] = ChunkCube{
                            .state = ChunkCube::State::NOT_PRESENT,
                        };
                        continue;
                    }
                    adjCubes[del] = cubes[x1][y1][z1];
                }

                auto sideTile = grassSideTile;
                if (y != BlockCount - 1) {
                    sideTile = dirtTile;
                }

                Cube cube(glm::vec3(x, y, z),
                          glm::vec3(BlockSize, BlockSize, BlockSize),
                          tilesetAtlas,
                          grassTile,
                          dirtTile,
                          sideTile,
                          0);

                auto cubeVertices = cube.Vertices();

                for (int del = 0; del < 6; del++) {
                    if (adjCubes[del].state == ChunkCube::State::PRESENT) {
                        continue;
                    }
                    for (int idx = del * 4; idx < del * 4 + 4; idx++) {
                        mesh.push_back(cubeVertices[idx]);
                    }
                    auto faceIndices = cube.FaceIndices(prevIndex);
                    for (auto faceIdx : faceIndices) {
                        indices.push_back(faceIdx);
                    }
                    prevIndex += 4;
                }
            }
        }
    }

    VertexArray vao;
    vao.Bind();

    VertexBuffer vbo(mesh.data(), mesh.size() * sizeof(Vertex));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(1);
    vao.AddBuffer(vbo, layout);

    IndexBuffer ibo(indices.data(), indices.size());

    renderer.draw(vao, ibo, shader);
}
