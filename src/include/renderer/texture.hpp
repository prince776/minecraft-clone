#pragma once

#include <OpenGL/OpenGL.h>
#include <string>
class Texture {
  public:
    Texture(const std::string& path) noexcept;
    ~Texture() noexcept;

    void Bind(size_t slot = 0) const noexcept;
    void Unbind() const noexcept;

  private:
    const std::string filepath;
    GLuint glID;
    unsigned char* data;
    int width, height, BPP;
};
