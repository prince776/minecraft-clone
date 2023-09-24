#include "renderer/vertex-buffer.hpp"
#include "fmt/core.h"
#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <cstddef>

VertexBuffer::VertexBuffer(const void* data, size_t size) noexcept {
    GLCALL(glGenBuffers(1, &glID));
    Bind();
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    if (glID != 0) {
        GLCALL(glDeleteBuffers(1, &glID));
    }
}

void VertexBuffer::Bind() const noexcept {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, glID));
}

void VertexBuffer::Unbind() const noexcept {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
