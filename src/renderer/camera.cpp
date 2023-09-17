#include "renderer/camera.hpp"
#include "GLFW/glfw3.h"
#include "ext/matrix_float4x4.hpp"
#include "ext/matrix_transform.hpp"
#include "matrix.hpp"
#include <GLFW/glfw3.h>

glm::mat4 Camera::ViewMatrix() const noexcept {
    auto viewMat = glm::mat4(1.0f);
    viewMat      = glm::translate(viewMat, pos);

    return glm::inverse(viewMat);
}

void Camera::Move(const glm::vec3& delta) noexcept {
    pos += delta;
}

void Camera::HandleInput(GLFWwindow* window) noexcept {
    auto posDelta = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        posDelta.y += DefaultSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        posDelta.y -= DefaultSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        posDelta.x += DefaultSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        posDelta.x -= DefaultSpeed;
    }

    Move(posDelta);
}
