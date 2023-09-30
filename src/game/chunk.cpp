#include "game/chunk.hpp"
#include "fmt/core.h"
#include "game/cube.hpp"
#include "game/geometry.hpp"
#include "game/perlin.hpp"
#include "game/random.hpp"
#include "game/textute-atlas.hpp"
#include "game/tiles.hpp"
#include "game/world.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex-array.hpp"
#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <algorithm>
#include <array>
#include <random>
#include <sys/_types/_intptr_t.h>
#include <valarray>
#include <vector>

Chunk::Chunk(const glm::vec3& pos) noexcept : pos(pos), generateMesh(true), vao(true) {
    cubes           = viii(BlockCount, vii(BlockCount, vi(BlockCount)));
    int waterHeight = 8;

    for (int x = 0; x < BlockCount; x++) {
        for (int z = 0; z < BlockCount; z++) {

            int tx = x + pos.x;
            int tz = z + pos.z;

            float height = (perlin::perlin((tx) / 15.0f + 10000, (tz) / 15.0f + 10000) + 1.3) *
                           0.5 * BlockCount;
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
                    if (y < stoneHeight) {
                        cubes[x][y][z].tilemap = stoneTileMap;
                    } else if (y < sandHeight) {
                        cubes[x][y][z].tilemap = sandTileMap;
                    } else {
                        cubes[x][y][z].tilemap = dirtTileMap;
                    }

                    if (y == heightInt - 1 && y > 6) {
                        cubes[x][y][z].tilemap = grassTileMap;
                    }
                } else if (y < waterHeight) {
                    cubes[x][y][z].state   = ChunkCube::State::PRESENT;
                    cubes[x][y][z].tilemap = waterTileMap;
                    cubes[x][y][z].alpha   = 0.2f;
                }
            }
        }
    }
}

// This ordering is coupled with the ordering we get from Cube::Vertices().
static int delX[] = {0, 0, 0, 0, -1, 1};
static int delY[] = {1, -1, 0, 0, 0, 0};
static int delZ[] = {0, 0, 1, -1, 0, 0};

void Chunk::Tick(World& world) noexcept {
    if (vao.GLID() == 0 || ibo.GLID() == 0 || vbo.GLID() == 0) {
        generateMesh = true;
    }
    if (generateMesh) {
        GenerateMesh(world);
        generateMesh = false;
    }
}

void Chunk::Render(const Renderer& renderer, const Shader& shader, World& world) noexcept {
    vao.Bind();
    ibo.Bind();
    renderer.draw(vao, ibo, shader);
}

void Chunk::RenderWater(const Renderer& renderer, const Shader& shader, World& world) noexcept {
    waterVAO.Bind();
    waterIBO.Bind();
    renderer.draw(waterVAO, waterIBO, shader);
}

void Chunk::GenerateMesh(World& world) noexcept {
    std::vector<Vertex> mesh, waterMesh;
    std::vector<unsigned int> indices, waterIndices;
    int prevIndex = 0, waterPrevIndex = 0;

    auto intPos     = Pos();
    auto leftChunk  = world.LookUpChunk(intPos.x - BlockCount, intPos.y, intPos.z);
    auto rightChunk = world.LookUpChunk(intPos.x + BlockCount, intPos.y, intPos.z);
    auto frontChunk = world.LookUpChunk(intPos.x, intPos.y, intPos.z + BlockCount);
    auto backChunk  = world.LookUpChunk(intPos.x, intPos.y, intPos.z - BlockCount);

    // fmt::println("Adjacent chunks: {} {} {} {}",
    //              (intptr_t)leftChunk,
    //              (intptr_t)rightChunk,
    //              (intptr_t)frontChunk,
    //              (intptr_t)backChunk);

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
                        if (x1 == -1 && leftChunk != nullptr) {
                            adjCubes[del] = leftChunk->cubes[BlockCount - 1][y][z];
                        } else if (x1 == BlockCount && rightChunk != nullptr) {
                            adjCubes[del] = rightChunk->cubes[0][y][z];
                        } else if (z1 == -1 && backChunk != nullptr) {
                            adjCubes[del] = backChunk->cubes[x][y][BlockCount - 1];
                        } else if (z1 == BlockCount && frontChunk != nullptr) {
                            adjCubes[del] = frontChunk->cubes[x][y][0];
                        } else {
                            adjCubes[del] = ChunkCube{
                                .state = ChunkCube::State::NOT_PRESENT,
                            };
                        }
                        continue;
                    }
                    adjCubes[del] = cubes[x1][y1][z1];
                }

                Cube cube(glm::vec3(x, y, z) + pos,
                          glm::vec3(BlockSize, BlockSize, BlockSize),
                          tilesetAtlas,
                          cubes[x][y][z].tilemap.topTile,
                          cubes[x][y][z].tilemap.bottomTile,
                          cubes[x][y][z].tilemap.sideTile,
                          0);

                auto cubeVertices = cube.Vertices(TransparentWhite(cubes[x][y][z].alpha));

                bool isWater = std::abs(cubes[x][y][z].tilemap.topTile.x - waterTile.x) < 1e-3 &&
                               std::abs(cubes[x][y][z].tilemap.topTile.y - waterTile.y) < 1e-3;

                for (int del = 0; del < 6; del++) {
                    bool shouldSkipSide = false;
                    if (cubes[x][y][z].IsTransparent()) {
                        shouldSkipSide = adjCubes[del].state == ChunkCube::State::PRESENT;
                    } else {
                        shouldSkipSide = adjCubes[del].state == ChunkCube::State::PRESENT &&
                                         !adjCubes[del].IsTransparent();
                    }

                    if (shouldSkipSide) {
                        continue;
                    }

                    for (int idx = del * 4; idx < del * 4 + 4; idx++) {
                        if (!isWater) {
                            mesh.push_back(cubeVertices[idx]);
                        } else {
                            waterMesh.push_back(cubeVertices[idx]);
                        }
                    }

                    auto pi = prevIndex;
                    if (isWater) {
                        pi = waterPrevIndex;
                    }
                    auto faceIndices = cube.FaceIndices(pi);
                    for (auto faceIdx : faceIndices) {
                        if (!isWater) {
                            indices.push_back(faceIdx);
                        } else {
                            waterIndices.push_back(faceIdx);
                        }
                    }

                    if (!isWater) {
                        prevIndex += 4;
                    } else {
                        waterPrevIndex += 4;
                    }
                }
            }
        }
    }

    {
        // vao.~VertexArray();
        VertexArray _vao;
        vao = std::move(_vao);
        _vao.Clean();

        vao.Bind();

        // vbo.~VertexBuffer();
        VertexBuffer _vbo = VertexBuffer(mesh.data(), mesh.size() * sizeof(Vertex));
        vbo               = std::move(_vbo);
        // TODO: Ideally this clean should happen in move constructor and assignment
        // operator.
        _vbo.Clean();

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);
        layout.Push<float>(4);

        vao.AddBuffer(vbo, layout);
        // TODO: Ideally this clean should happen in move constructor and assignment
        // operator.
        // ibo.~IndexBuffer();
        IndexBuffer _ibo(indices.data(), indices.size());
        ibo = std::move(_ibo);
        _ibo.Clean();
    }

    {
        // waterVAO.~VertexArray();
        VertexArray _vao;
        waterVAO = std::move(_vao);
        _vao.Clean();

        waterVAO.Bind();

        // waterVBO.~VertexBuffer();
        VertexBuffer _vbo = VertexBuffer(waterMesh.data(), waterMesh.size() * sizeof(Vertex));
        waterVBO          = std::move(_vbo);
        // TODO: Ideally this clean should happen in move constructor and assignment
        // operator.
        _vbo.Clean();

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);
        layout.Push<float>(4);

        waterVAO.AddBuffer(waterVBO, layout);
        // TODO: Ideally this clean should happen in move constructor and assignment
        // operator.
        // waterIBO.~IndexBuffer();
        IndexBuffer _ibo(waterIndices.data(), waterIndices.size());
        waterIBO = std::move(_ibo);
        _ibo.Clean();
    }
}
