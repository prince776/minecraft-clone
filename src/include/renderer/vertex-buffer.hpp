#pragma once

#include "renderer/core.hpp"

class VertexBuffer {
  public:
    VertexBuffer() = default;
    VertexBuffer(const void* data, size_t size) noexcept;
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer& other) {
        glID = other.glID;
    }
    VertexBuffer& operator=(const VertexBuffer& other) {
        glID = other.glID;
        return *this;
    }

    VertexBuffer(VertexBuffer&& other) noexcept {
        glID       = other.glID;
        other.glID = 0;
    }
    VertexBuffer& operator=(VertexBuffer&& other) noexcept {
        glID       = other.glID;
        other.glID = 0;

        return *this;
    }

    void Bind() const noexcept;
    void Unbind() const noexcept;

    GLuint GLID() const noexcept {
        return glID;
    }

    void Clean() noexcept {
        glID = 0;
    }

  private:
    GLuint glID{};
};
