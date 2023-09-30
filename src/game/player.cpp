#include "game/player.hpp"
#include "GLFW/glfw3.h"
#include "fmt/core.h"
#include "game/chunk.hpp"
#include "game/tiles.hpp"
#include <cmath>
#include <future>
#include <thread>

void Player::Move(const glm::vec3& delta) noexcept {
    pos += delta;
}

void Player::Rotate(const glm::vec3& delta) noexcept {
    rot += delta;
}

void Player::HandleInput(GLFWwindow* window, double deltaTime) noexcept {
    auto speed    = deltaTime * DefaultSpeed;
    auto posDelta = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        posDelta.z += speed * std::cos(rot.y);
        posDelta.x += speed * std::sin(rot.y);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        posDelta.z -= speed * std::cos(rot.y);
        posDelta.x -= speed * std::sin(rot.y);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        posDelta.x += speed * std::cos(rot.y);
        posDelta.z -= speed * std::sin(rot.y);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        posDelta.x -= speed * std::cos(rot.y);
        posDelta.z += speed * std::sin(rot.y);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        posDelta.y -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        posDelta.y += speed;
    }

    Move(posDelta);

    auto rotDelta = glm::vec3(0.0f);

    double currX, currY;
    glfwGetCursorPos(window, &currX, &currY);

    if (std::abs(mouseX + 1) < 1e-6) {
        mouseX = currX;
        mouseY = currY;
    }

    auto delX = currX - mouseX;
    auto delY = currY - mouseY;

    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
    rotDelta.y = -delX * 0.001f;
    rotDelta.x = -delY * 0.001f;
    // }

    mouseX = currX;
    mouseY = currY;

    Rotate(rotDelta);

    for (int i = 0; i < 8; i++) {
        if (glfwGetKey(window, GLFW_KEY_0 + i)) {
            if (selectedBlockToPlace != i) {
                fmt::println("Selecting block to place: {}", i);
            }
            selectedBlockToPlace = i;
            break;
        }
    }
}

// x + sinTheta
// y - sinPhi
// z - cosTheta - costPhi
// Theta = rotY
// Phi = rotX

void Player::Tick(GLFWwindow* window, double deltaTime, World& world) noexcept {
    HandleInput(window, deltaTime);

    // auto generateWorld = [&]() {
    if (worldGenChecker == 0) {
        for (auto deltaX : {-16, 16, 0, 32, -32}) {
            for (auto deltaZ : {-16, 16, 0, 32, -32}) {
                auto playerChunkPos = pos;
                playerChunkPos.y    = 0;
                playerChunkPos.x += deltaX;
                playerChunkPos.z += deltaZ;

                auto [_, playerChunk] = world.BlockAt(playerChunkPos);

                if (!playerChunk) {
                    int x = playerChunkPos.x;
                    int z = playerChunkPos.z;

                    int chunkX = (x / Chunk::BlockCount) * Chunk::BlockCount;
                    int chunkZ = (z / Chunk::BlockCount) * Chunk::BlockCount;
                    if (x < 0)
                        chunkX -= Chunk::BlockCount;
                    if (z < 0)
                        chunkZ -= Chunk::BlockCount;

                    world.AddChunk(chunkX, chunkZ);

                    auto leftChunk  = world.LookUpChunk(chunkX - Chunk::BlockCount, 0, chunkZ);
                    auto rightChunk = world.LookUpChunk(chunkX + Chunk::BlockCount, 0, chunkZ);
                    auto frontChunk = world.LookUpChunk(chunkX, 0, chunkZ - Chunk::BlockCount);
                    auto backChunk  = world.LookUpChunk(chunkX, 0, chunkZ + Chunk::BlockCount);

                    if (leftChunk)
                        leftChunk->generateMesh = true;
                    if (rightChunk)
                        rightChunk->generateMesh = true;
                    if (frontChunk)
                        frontChunk->generateMesh = true;
                    if (backChunk)
                        backChunk->generateMesh = true;
                }
            }
        }
        worldGenChecker = worldGenCooldown;
    }
    // };

    // auto _ = std::async(generateWorld);

    if (blockPlacementChecker == 0) {
        // Calculate the direction vector
        auto pitch = rot.x, yaw = -rot.y;

        glm::vec3 direction;
        direction.x = cos(pitch) * sin(yaw);
        direction.y = sin(pitch);
        direction.z = -cos(pitch) * cos(yaw);

        direction = glm::normalize(direction);

        for (float reach = 1; reach <= 7; reach += 0.5f) {
            auto rayVec            = pos + reach * direction;
            auto [blockPos, chunk] = world.BlockAt(rayVec);
            if (chunk) {
                auto& cube          = chunk->cubes[blockPos.x][blockPos.y][blockPos.z];
                Chunk* toRegenerate = nullptr;
                if (cube.state == ChunkCube::State::PRESENT) {
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
                        cube.state   = ChunkCube::State::NOT_PRESENT;
                        toRegenerate = chunk;

                        // // fmt::println("Ray at: {}, {} {}: {}", rayVec.x, rayVec.y, rayVec.z,
                        // reach);
                        // // fmt::println("Block selected: {} {} {}",
                        //              blockPos.x,
                        //              blockPos.y,
                        //              blockPos.z);
                    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
                        rayVec                                   = pos + (reach - 1) * direction;
                        auto [placementBlockPos, placementChunk] = world.BlockAt(rayVec);

                        if (placementChunk) {
                            auto& placementCube =
                                chunk->cubes[placementBlockPos.x][placementBlockPos.y]
                                            [placementBlockPos.z];

                            if (placementCube.state == ChunkCube::State::PRESENT) {
                                fmt::println("shouldn't happen");
                            }

                            placementCube.tilemap = tilePlacementMap[selectedBlockToPlace];
                            placementCube.state   = ChunkCube::State::PRESENT;
                            if (selectedBlockToPlace == 5) {
                                placementCube.alpha = 0.2f;
                            } else {
                                placementCube.alpha = 1;
                            }

                            toRegenerate = placementChunk;
                        }
                    }

                    if (toRegenerate) {
                        toRegenerate->generateMesh = true;
                        auto chunkPos              = toRegenerate->Pos();
                        auto leftChunk  = world.LookUpChunk(chunkPos.x - Chunk::BlockCount,
                                                           chunkPos.y,
                                                           chunkPos.z);
                        auto rightChunk = world.LookUpChunk(chunkPos.x + Chunk::BlockCount,
                                                            chunkPos.y,
                                                            chunkPos.z);
                        auto frontChunk = world.LookUpChunk(chunkPos.x,
                                                            chunkPos.y,
                                                            chunkPos.z + Chunk::BlockCount);
                        auto backChunk  = world.LookUpChunk(chunkPos.x,
                                                           chunkPos.y,
                                                           chunkPos.z - Chunk::BlockCount);

                        if (leftChunk && blockPos.x == 0) {
                            leftChunk->generateMesh = true;
                        }
                        if (rightChunk && blockPos.x == Chunk::BlockCount - 1) {
                            rightChunk->generateMesh = true;
                        }
                        if (frontChunk && blockPos.z == Chunk::BlockCount - 1) {
                            frontChunk->generateMesh = true;
                        }
                        if (backChunk && blockPos.z == 0) {
                            backChunk->generateMesh = true;
                        }

                        blockPlacementChecker += blockPlacementCooldown;
                        break;
                    }
                }
            }
        }
    }
}

void Player::Render(Renderer& renderer, Shader& shader) noexcept {
    if (worldGenChecker > 0) {
        worldGenChecker--;
    }
    if (blockPlacementChecker > 0) {
        blockPlacementChecker--;
    }
}
