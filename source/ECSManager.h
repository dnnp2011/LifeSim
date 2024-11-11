#pragma once

#include <unordered_map>
#include <vector>

#include "CollisionSystem.h"
#include "Common.h"
#include "MovementSystem.h"
#include "RenderSystem.h"


class ECSManager {
public:
    EntityBuffer m_Entities;
    PositionBuffer m_Positions;
    VelocityBuffer m_Velocities;
    ColliderBuffer m_Colliders;
    ShapeTypeBuffer m_Shapes;

    MovementSystem m_MovementSystem;
    CollisionSystem m_CollisionSystem;
    RenderSystem m_RenderSystem;

    ECSManager();

    Entity createEntity(const Position& position, const Velocity& velocity, const Collider& collider, const ShapeType& shape);

    void addComponent(int entityId, const Position& position);

    void addComponent(int entityId, const Velocity& velocity);

    void addComponent(int entityId, const Collider& collider);

    void addComponent(int entityId, const ShapeType& shape);

    void update(float fixedDeltaTime);
};
