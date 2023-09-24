#include "renderer/camera.hpp"
#include "GLFW/glfw3.h"
#include "ext/matrix_float4x4.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/vector_float3.hpp"
#include "fmt/core.h"
#include "matrix.hpp"
#include <GLFW/glfw3.h>
#include <valarray>

glm::mat4 Camera::ViewMatrix() const noexcept {
    auto viewMat = glm::mat4(1.0f);
    viewMat      = glm::translate(viewMat, pos);

    // Wow, we have to do y axis rot before x, otherwise it inverts sometimes.
    auto yAxis = glm::vec3(0, 1, 0);
    viewMat    = glm::rotate(viewMat, rot.y, yAxis);

    auto xAxis = glm::vec3(1, 0, 0);
    viewMat    = glm::rotate(viewMat, rot.x, xAxis);

    auto zAxis = glm::vec3(0, 0, 1);
    viewMat    = glm::rotate(viewMat, rot.z, zAxis);

    return glm::inverse(viewMat);
}

void Camera::Move(const glm::vec3& delta) noexcept {
    pos += delta;
}

void Camera::Rotate(const glm::vec3& delta) noexcept {
    rot += delta;
}

void Camera::HandleInput(GLFWwindow* window, double deltaTime) noexcept {
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
