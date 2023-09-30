#pragma once

#include "GLFW/glfw3.h"
#include "ext/matrix_float4x4.hpp"
#include "ext/vector_float3.hpp"
class Camera {
    Camera() noexcept     = default;
    Camera(const Camera&) = delete;

  public:
    static Camera& Get() noexcept {
        static Camera camera;

        return camera;
    }

    glm::mat4 ViewMatrix() const noexcept;

    void SetPos(const glm::vec3& newPos) noexcept {
        pos = newPos;
    }

    void SetRot(const glm::vec3& newRot) noexcept {
        rot = newRot;
    }

  private:
    glm::vec3 pos;
    glm::vec3 rot;

    double mouseX = -1, mouseY = -1;

  public:
    static inline float DefaultSpeed = 15.0f;
};
