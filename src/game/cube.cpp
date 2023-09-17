#include "game/cube.hpp"
#include "game/textute-atlas.hpp"
#include <array>

Cube::Cube(glm::vec3 pos,
           glm::vec3 dim,
           TextureAtlas textureAtlas,
           TexCoord topTexture,
           TexCoord bottomTexture,
           TexCoord sideTexture,
           size_t texSlot) noexcept
    : pos(pos), dim(dim), textureAtlas(textureAtlas), topTexture(topTexture),
      bottomTexture(bottomTexture), sideTexture(sideTexture), texSlot(texSlot) {
}

[[nodiscard]] std::array<unsigned int, 36> Cube::Indices() const noexcept {
    std::array<unsigned int, 36> indices;
    for (int quadCount = 0; quadCount < 6; quadCount++) {
        indices[quadCount * 6 + 0] = quadCount * 4 + 0;
        indices[quadCount * 6 + 1] = quadCount * 4 + 1;
        indices[quadCount * 6 + 2] = quadCount * 4 + 2;
        indices[quadCount * 6 + 3] = quadCount * 4 + 2;
        indices[quadCount * 6 + 4] = quadCount * 4 + 3;
        indices[quadCount * 6 + 5] = quadCount * 4 + 0;
    }

    return indices;
}

[[nodiscard]] std::array<Vertex, 32> Cube::Vertices() const noexcept {
    auto halfDim     = dim * 0.5f;
    glm::vec3 center = pos + halfDim;

    auto topTextureCoords    = textureAtlas.TexCoordsForTexture(topTexture);
    auto bottomTextureCoords = textureAtlas.TexCoordsForTexture(bottomTexture);
    auto sideTextureCoords   = textureAtlas.TexCoordsForTexture(sideTexture);

    return std::array<Vertex, 32>({
  // top face.
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y + halfDim.y,
               center.z + halfDim.z,
               },    .tex     = topTextureCoords[0],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y + halfDim.y,
               center.z + halfDim.z,
               },    .tex     = topTextureCoords[1],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y + halfDim.y,
               center.z - halfDim.z,
               },    .tex     = topTextureCoords[2],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y + halfDim.y,
               center.z - halfDim.z,
               },    .tex     = topTextureCoords[3],
               .texSlot = texSlot,
               },
 // bottom face.
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y - halfDim.y,
               center.z + halfDim.z,
               }, .tex     = bottomTextureCoords[0],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y - halfDim.y,
               center.z + halfDim.z,
               }, .tex     = bottomTextureCoords[1],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y - halfDim.y,
               center.z - halfDim.z,
               }, .tex     = bottomTextureCoords[2],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y - halfDim.y,
               center.z - halfDim.z,
               }, .tex     = bottomTextureCoords[3],
               .texSlot = texSlot,
               },
 // front face.
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y - halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[0],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y - halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[1],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y + halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[2],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y + halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[3],
               .texSlot = texSlot,
               },
 // back face.
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y - halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[0],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y - halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[1],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y + halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[2],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y + halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[3],
               .texSlot = texSlot,
               },
 // left face.
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y - halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[0],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y - halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[1],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y + halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[2],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x - halfDim.x,
               center.y + halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[3],
               .texSlot = texSlot,
               },
 // right face.
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y - halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[0],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y - halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[1],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y + halfDim.y,
               center.z + halfDim.z,
               },   .tex     = sideTextureCoords[2],
               .texSlot = texSlot,
               },
        Vertex{
               .pos =
               Vec3{
               center.x + halfDim.x,
               center.y + halfDim.y,
               center.z - halfDim.z,
               },   .tex     = sideTextureCoords[3],
               .texSlot = texSlot,
               },
    });
}
