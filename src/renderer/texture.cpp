#include "renderer/texture.hpp"
#include "renderer/core.hpp"
#include "vendor/stb/stb-image.h"
#include <OpenGL/OpenGL.h>

Texture::Texture(const std::string& path) noexcept
    : filepath(path), glID(0), data(nullptr), width(0), height(0), BPP(0) {
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path.c_str(), &width, &height, &BPP, 4);

    GLCALL(glGenTextures(1, &glID));
    GLCALL(glBindTexture(GL_TEXTURE_2D, glID));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(GL_TEXTURE_2D,
                        0,
                        GL_RGBA8,
                        width,
                        height,
                        0,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        (const void*)data));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (data) {
        stbi_image_free(data);
    }
}

Texture::~Texture() noexcept {
    GLCALL(glDeleteTextures(1, &glID));
}

void Texture::Bind(size_t slot) const noexcept {
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, glID));
}

void Texture::Unbind() const noexcept {
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
