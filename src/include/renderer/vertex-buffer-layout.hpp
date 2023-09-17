#pragma once

#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <vector>

struct VertexBufferElement {
    GLenum type;
    size_t count;
    unsigned char normalized;

    VertexBufferElement(GLenum type, size_t count, unsigned char normalized)
        : type(type), count(count), normalized(normalized) {
    }

    static size_t GetSizeOfType(size_t type) {
        switch (type) {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
  public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    template <typename T>
    void Push(int count);

    [[nodiscard]] size_t Stride() const {
        return stride;
    }

    [[nodiscard]] const std::vector<VertexBufferElement>& Elements() const {
        return elements;
    }

  private:
    size_t stride;
    std::vector<VertexBufferElement> elements;
};
