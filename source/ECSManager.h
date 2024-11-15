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

private:
    MovementSystem m_MovementSystem{ std::thread::hardware_concurrency() };
    CollisionSystem m_CollisionSystem{ std::thread::hardware_concurrency() };
    RenderSystem m_RenderSystem;

public:
    ECSManager();

    void update(float fixedDeltaTime);

private:
    Entity createEntity(const Position& position, const Velocity& velocity, const Collider& collider, const ShapeType& shape);

    void addComponent(int entityId, const Position& position);

    void addComponent(int entityId, const Velocity& velocity);

    void addComponent(int entityId, const Collider& collider);

    void addComponent(int entityId, const ShapeType& shape);
};
