#include "renderer/renderer.hpp"
#include "fmt/core.h"
#include "renderer/core.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex-array.hpp"
#include <OpenGL/OpenGL.h>

void Renderer::clear() const noexcept {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
}

void Renderer::draw(const VertexArray& vao,
                    const IndexBuffer& ibo,
                    const Shader& shader) const noexcept {
    shader.Bind();
    vao.Bind();
    ibo.Bind();

    // fmt::println("Drawing {} indices", ibo.Count());

    GLCALL(glDrawElements(GL_TRIANGLES, ibo.Count(), GL_UNSIGNED_INT, nullptr));
}
