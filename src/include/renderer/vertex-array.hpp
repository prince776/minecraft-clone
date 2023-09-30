#pragma once

#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <OpenGL/OpenGL.h>
#include <OpenGL/gltypes.h>

class VertexArray {
  public:
    VertexArray(bool doNothing /* = should be always tru*/) noexcept {
    }
    VertexArray() noexcept;
    ~VertexArray() noexcept;

    VertexArray(const VertexArray& other) {
        glID = other.glID;
    }
    VertexArray& operator=(const VertexArray& other) {
        glID = other.glID;
        return *this;
    }

    VertexArray(VertexArray&& other) noexcept {
        glID       = other.glID;
        other.glID = 0;
    }
    VertexArray& operator=(VertexArray&& other) noexcept {
        glID       = other.glID;
        other.glID = 0;

        return *this;
    }

    void Bind() const noexcept;
    void Unbind() const noexcept;
    void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) noexcept;

    GLuint GLID() const noexcept {
        return glID;
    }

    void Clean() noexcept {
        glID = 0;
    }

  private:
    GLuint glID{};
};
