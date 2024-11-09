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
    std::vector<Entity> m_Entities;
    std::unordered_map<int, Position> m_Positions;
    std::unordered_map<int, Velocity> m_Velocities;
    std::unordered_map<int, Collider> m_Colliders;
    std::unordered_map<int, ShapeType> m_Shapes;

    MovementSystem m_MovementSystem;
    CollisionSystem m_CollisionSystem;
    RenderSystem m_RenderSystem;

    ECSManager();

    Entity createEntity(Position position, Velocity velocity, Collider collider, ShapeType shape);

    void addComponent(int entityId, Position position);

    void addComponent(int entityId, Velocity velocity);

    void addComponent(int entityId, Collider collider);

    void addComponent(int entityId, ShapeType shape);

    void update(float fixedDeltaTime);
};
