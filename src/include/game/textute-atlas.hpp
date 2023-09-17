#pragma once

#include "game/geometry.hpp"
#include <array>

inline static float floatDiv(int x, int y) {
    return (float)x / (float)y;
}

using TexCoord = Vec2;

struct TextureAtlas {
    int countX, countY;

    TextureAtlas(int countX, int countY) : countX(countX), countY(countY) {
    }

    std::array<TexCoord, 4> TexCoordsForTexture(TexCoord texCoord) const noexcept {
        return std::array<TexCoord, 4>({
            TexCoord{    floatDiv(texCoord.x, countX),     floatDiv(texCoord.y, countY)},
            TexCoord{floatDiv(texCoord.x + 1, countX),     floatDiv(texCoord.y, countY)},
            TexCoord{floatDiv(texCoord.x + 1, countX), floatDiv(texCoord.y + 1, countY)},
            TexCoord{    floatDiv(texCoord.x, countX), floatDiv(texCoord.y + 1, countY)},
        });
    }
};
