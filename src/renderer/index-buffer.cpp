#include "renderer/index-buffer.hpp"
#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>

IndexBuffer::IndexBuffer(const unsigned int* data, size_t count) noexcept
    : count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCALL(glGenBuffers(1, &glID));
    Bind();
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        count * sizeof(unsigned int),
                        data,
                        GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() noexcept {
    GLCALL(glDeleteBuffers(1, &glID));
}

void IndexBuffer::Bind() const noexcept {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glID));
}

void IndexBuffer::Unbind() const noexcept {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
