#pragma once

#include <imgui.h>


struct Velocity {
    float dx, dy;
};

struct Collider {
    int width, height;
};

struct Entity {
    int id;
};

enum class ShapeType {
    Rectangle,
    Circle,
    Triangle,
};
