#pragma once

struct Vec4 {
    float x, y, z, w;
};

static constexpr inline Vec4 WhiteColor = Vec4{1, 1, 1, 1};

constexpr inline Vec4 TransparentWhite(float alpha) {
    return Vec4{
        1,
        1,
        1,
        alpha,
    };
}

struct Vec3 {
    float x, y, z;
};

struct Vec2 {
    float x, y;
};

struct Vertex {
    Vec3 pos;
    Vec2 tex;
    float texSlot;
    Vec4 color;
};

static_assert(sizeof(Vertex) == 40);
