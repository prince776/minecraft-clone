#include "game/chunk.hpp"
#include "fmt/core.h"
#include "game/cube.hpp"
#include "game/perlin.hpp"
#include "game/random.hpp"
#include "game/textute-atlas.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex-array.hpp"
#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <algorithm>
#include <array>
#include <random>
#include <vector>

Chunk::Chunk(const glm::vec3& pos) noexcept : pos(pos), generateMesh(true), vao(true) {
    TextureAtlas tilesetAtlas(16, 16);
    TexCoord dirtTile{2, 15}, grassTile{0, 15}, grassSideTile{3, 15};
    TexCoord stoneTile{1, 15}, sandTile{0, 4};

    cubes = viii(BlockCount, vii(BlockCount, vi(BlockCount)));
    for (int x = 0; x < BlockCount; x++) {
        for (int z = 0; z < BlockCount; z++) {

            int tx = x + pos.x;
            int tz = z + pos.z;

            float height  = (perlin::perlin((tx) / 15.0f, (tz) / 15.0f) + 1.3) * 0.5 * BlockCount;
            int heightInt = std::max(5, int(height));

            std::mt19937 rng(tx + 16 * tz + heightInt * 16 * 16);
            float stoneHeight = getRandomNumber(rng, 3, 5);
            float sandHeight  = stoneHeight + getRandomNumber(rng, 1, 2);

            for (int y = 0; y < BlockCount; y++) {
                cubes[x][y][z] = ChunkCube{
                    .state = ChunkCube::State::NOT_PRESENT,
                };
                if (y < heightInt) {
                    cubes[x][y][z].state = ChunkCube::State::PRESENT;
                }

                if (y < stoneHeight) {
                    cubes[x][y][z].sideTile   = stoneTile;
                    cubes[x][y][z].topTile    = stoneTile;
                    cubes[x][y][z].bottomTile = stoneTile;
                } else if (y < sandHeight) {
                    cubes[x][y][z].sideTile   = sandTile;
                    cubes[x][y][z].topTile    = sandTile;
                    cubes[x][y][z].bottomTile = sandTile;
                } else {
                    cubes[x][y][z].sideTile   = dirtTile;
                    cubes[x][y][z].topTile    = dirtTile;
                    cubes[x][y][z].bottomTile = dirtTile;
                }

                if (y == heightInt - 1 && y > 6) {
                    cubes[x][y][z].sideTile   = grassSideTile;
                    cubes[x][y][z].topTile    = grassTile;
                    cubes[x][y][z].bottomTile = dirtTile;
                }
            }
        }
    }
}

// This ordering is coupled with the ordering we get from Cube::Vertices().
static int delX[] = {0, 0, 0, 0, -1, 1};
static int delY[] = {1, -1, 0, 0, 0, 0};
static int delZ[] = {0, 0, 1, -1, 0, 0};

void Chunk::Render(const Renderer& renderer, const Shader& shader) noexcept {
    if (generateMesh) {
        std::vector<Vertex> mesh;
        std::vector<unsigned int> indices;
        int prevIndex = 0;

        TextureAtlas tilesetAtlas(16, 16);
        TexCoord dirtTile{2, 15}, grassTile{0, 15}, grassSideTile{3, 15};
        TexCoord stoneTile{1, 15}, sandTile{0, 4};

        for (int x = 0; x < BlockCount; x++) {
            for (int z = 0; z < BlockCount; z++) {
                for (int y = 0; y < BlockCount; y++) {
                    if (cubes[x][y][z].state == ChunkCube::State::NOT_PRESENT) {
                        continue;
                    }

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

                    Cube cube(glm::vec3(x, y, z) + pos,
                              glm::vec3(BlockSize, BlockSize, BlockSize),
                              tilesetAtlas,
                              cubes[x][y][z].topTile,
                              cubes[x][y][z].bottomTile,
                              cubes[x][y][z].sideTile,
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

        VertexArray _vao;
        vao = std::move(_vao);
        _vao.Clean();

        vao.Bind();

        VertexBuffer _vbo = VertexBuffer(mesh.data(), mesh.size() * sizeof(Vertex));
        vbo               = std::move(_vbo);
        // TODO: Ideally this clean should happen in move constructor and assignment operator.
        _vbo.Clean();

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);

        vao.AddBuffer(vbo, layout);
        // TODO: Ideally this clean should happen in move constructor and assignment operator.
        IndexBuffer _ibo(indices.data(), indices.size());
        ibo = std::move(_ibo);
        _ibo.Clean();

        generateMesh = false;
    }

    vao.Bind();
    ibo.Bind();
    renderer.draw(vao, ibo, shader);
}
