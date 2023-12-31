#pragma once

#include "ext/vector_float3.hpp"
#include "game/geometry.hpp"
#include "game/textute-atlas.hpp"

class Cube {
  public:
    Cube(glm::vec3 pos,
         glm::vec3 dim,
         TextureAtlas textureAtlas,
         TexCoord topTexture,
         TexCoord bottomTexture,
         TexCoord sideTexture,
         size_t texSlot) noexcept;

    [[nodiscard]] std::array<Vertex, 32> Vertices(Vec4 color = WhiteColor) const noexcept;

    [[nodiscard]] std::array<unsigned int, 36> Indices() const noexcept;

    [[nodiscard]] std::array<unsigned int, 6> FaceIndices(unsigned int startIdx) const noexcept;

  private:
    glm::vec3 pos;
    glm::vec3 dim;

    TextureAtlas textureAtlas;
    TexCoord topTexture, bottomTexture, sideTexture;
    float texSlot;
};
