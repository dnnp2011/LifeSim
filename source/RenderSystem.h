#pragma once

#include <unordered_map>
#include <vector>

#include "common.h"


class RenderSystem {
public:
    void update(const std::vector<Entity> &entities, const std::unordered_map<int, Position> &positions, const std::unordered_map<int, Collider> &colliders, const std::unordered_map<int, ShapeType> &shapes);
};
