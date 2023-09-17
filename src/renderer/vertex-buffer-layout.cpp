#include "renderer/vertex-buffer-layout.hpp"
#include <OpenGL/OpenGL.h>

VertexBufferLayout::VertexBufferLayout() : stride(0) {
}

VertexBufferLayout::~VertexBufferLayout() {
}

template <typename T>
void VertexBufferLayout::Push(int) {
    ASSERT(false);
}

template <>
void VertexBufferLayout::Push<float>(int count) {
    elements.emplace_back(GL_FLOAT, count, GL_FALSE);
    stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template <>
void VertexBufferLayout::Push<unsigned int>(int count) {
    elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
    stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template <>
void VertexBufferLayout::Push<unsigned char>(int count) {
    elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE);
    stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
