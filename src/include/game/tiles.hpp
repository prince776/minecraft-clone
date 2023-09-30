#pragma once

#include "game/textute-atlas.hpp"
#include <array>

struct TileMap {
    TexCoord topTile;
    TexCoord bottomTile;
    TexCoord sideTile;
};

constexpr TileMap SingleTileMap(TexCoord tile) {
    return TileMap{tile, tile, tile};
}

inline TextureAtlas tilesetAtlas{16, 16};
inline TexCoord dirtTile{2, 15}, grassTile{0, 15}, grassSideTile{3, 15};
inline TexCoord stoneTile{1, 15}, sandTile{0, 4}, waterTile{14, 15};
inline TexCoord woodTopTile{5, 14}, woodSideTile{4, 14};
inline TexCoord sandStoneTile{0, 3}, cobbleStoneTile{0, 14};

inline TileMap grassTileMap{grassTile, dirtTile, grassSideTile};
inline TileMap woodTileMap{woodTopTile, woodTopTile, woodSideTile};
inline TileMap dirtTileMap        = SingleTileMap(dirtTile);
inline TileMap stoneTileMap       = SingleTileMap(stoneTile);
inline TileMap waterTileMap       = SingleTileMap(waterTile);
inline TileMap sandTileMap        = SingleTileMap(sandTile);
inline TileMap sandStoneTileMap   = SingleTileMap(sandStoneTile);
inline TileMap cobbleStoneTileMap = SingleTileMap(cobbleStoneTile);
