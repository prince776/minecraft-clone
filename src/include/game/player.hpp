#pragma once

#include "GLFW/glfw3.h"
#include "ext/vector_float3.hpp"
#include "game/chunk.hpp"
#include "game/world.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"

class Player {
  public:
    Player(glm::vec3 pos) : pos(pos), vel(0), rot(0) {
    }

    glm::vec3 Pos() const noexcept {
        return pos;
    }

    glm::vec3 Rot() const noexcept {
        return rot;
    }

    void Move(const glm::vec3& delta) noexcept;
    void Rotate(const glm::vec3& delta) noexcept;

    void Tick(GLFWwindow* window, double deltaTime, World& world) noexcept;
    void HandleInput(GLFWwindow* window, double deltaTime) noexcept;

    void Render(Renderer& renderer, Shader& shader) noexcept;

  private:
    glm::vec3 pos, vel;
    glm::vec3 rot;

    double mouseX = -1, mouseY = -1;
    int blockPlacementCooldown = 10; // frames
    int blockPlacementChecker  = blockPlacementCooldown;

  public:
    static inline float DefaultSpeed = 15.0f;
};
