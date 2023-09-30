#include "game/player.hpp"
#include "GLFW/glfw3.h"
#include "fmt/core.h"
#include "game/chunk.hpp"
#include <cmath>

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
}

// x + sinTheta
// y - sinPhi
// z - cosTheta - costPhi
// Theta = rotY
// Phi = rotX

void Player::Tick(GLFWwindow* window, double deltaTime, World& world) noexcept {
    HandleInput(window, deltaTime);

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
                auto& cube = chunk->cubes[blockPos.x][blockPos.y][blockPos.z];
                if (cube.state == ChunkCube::State::PRESENT) {
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
                        cube.state          = ChunkCube::State::NOT_PRESENT;
                        chunk->generateMesh = true;

                        auto chunkPos   = chunk->Pos();
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

                        // // fmt::println("Ray at: {}, {} {}: {}", rayVec.x, rayVec.y, rayVec.z,
                        // reach);
                        // // fmt::println("Block selected: {} {} {}",
                        //              blockPos.x,
                        //              blockPos.y,
                        //              blockPos.z);

                        blockPlacementChecker += blockPlacementCooldown;
                        break;
                    }
                }
            }
        }
    }
}

void Player::Render(Renderer& renderer, Shader& shader) noexcept {
    if (blockPlacementChecker > 0) {
        blockPlacementChecker--;
    }
}
