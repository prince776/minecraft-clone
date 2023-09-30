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
