#include "game/chunk.hpp"
#include "game/cube.hpp"
#include "game/textute-atlas.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex-array.hpp"
#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"

Chunk::Chunk(const glm::vec3& pos) noexcept : pos(pos) {
}

void Chunk::Render(const Renderer& renderer, const Shader& shader) const noexcept {
    for (int x = 0; x < BlockCount; x++) {
        for (int z = 0; z < BlockCount; z++) {
            for (int y = 0; y < BlockCount; y++) {
                TextureAtlas tilesetAtlas(16, 16);
                TexCoord dirtTile{2, 15}, grassTile{0, 15}, grassSideTile{3, 15};

                Cube cube(glm::vec3(x, y, z),
                          glm::vec3(BlockSize, BlockSize, BlockSize),
                          tilesetAtlas,
                          dirtTile,
                          dirtTile,
                          dirtTile,
                          0);

                auto cubeVertices = cube.Vertices();
                auto cubeIndices  = cube.Indices();

                VertexArray vao;
                vao.Bind();

                VertexBuffer vbo(cubeVertices.data(), cubeVertices.size() * sizeof(Vertex));

                VertexBufferLayout layout;
                layout.Push<float>(3);
                layout.Push<float>(2);
                layout.Push<float>(1);
                vao.AddBuffer(vbo, layout);

                IndexBuffer ibo(cubeIndices.data(), cubeIndices.size());

                renderer.draw(vao, ibo, shader);
            }
        }
    }
}
