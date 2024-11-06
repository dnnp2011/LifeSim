#pragma once

#include <vector>
#include <unordered_map>

#include "Common.h"
#include "Renderer.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "RenderSystem.h"


class ECSManager {
public:
    std::vector<Entity> entities;
    std::unordered_map<int, Position> positions;
    std::unordered_map<int, Velocity> velocities;
    std::unordered_map<int, Collider> colliders;
    std::unordered_map<int, ShapeType> shapes;

    MovementSystem movementSystem;
    CollisionSystem collisionSystem;
    RenderSystem renderSystem;

    ECSManager();

    Entity createEntity(Position position, Velocity velocity, Collider collider, ShapeType shape);

    void addComponent(int entityId, Position position);

    void addComponent(int entityId, Velocity velocity);

    void addComponent(int entityId, Collider collider);

    void addComponent(int entityId, ShapeType shape);

    void update();
};
