#pragma once

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
};

static_assert(sizeof(Vertex) == 24);
