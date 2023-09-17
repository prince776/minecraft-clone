#include "renderer/vertex-array.hpp"
#include "renderer/core.hpp"
#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <OpenGL/OpenGL.h>

VertexArray::VertexArray() noexcept {
    GLCALL(glGenVertexArrays(1, &glID));
}

VertexArray::~VertexArray() noexcept {
    GLCALL(glDeleteVertexArrays(1, &glID));
}

void VertexArray::Bind() const noexcept {
    GLCALL(glBindVertexArray(glID));
}

void VertexArray::Unbind() const noexcept {
    GLCALL(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vbo,
                            const VertexBufferLayout& layout) noexcept {
    Bind();
    vbo.Bind();

    const auto& elements = layout.Elements();

    size_t offset = 0;
    size_t idx    = 0;
    for (const auto& element : elements) {
        GLCALL(glEnableVertexAttribArray(idx));
        GLCALL(glVertexAttribPointer(idx,
                                     element.count,
                                     element.type,
                                     element.normalized,
                                     layout.Stride(),
                                     (const void*)offset));
        offset +=
            element.count * VertexBufferElement::GetSizeOfType(element.type);
        idx++;
    }
}
