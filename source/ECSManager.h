#pragma once

#include <vector>
#include <unordered_map>

#include "Common.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
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

    Entity createEntity(Position position, Velocity velocity, Collider collider, ShapeType shape);

    void addComponent(int entityId, Position position);

    void addComponent(int entityId, Velocity velocity);

    void addComponent(int entityId, Collider collider);

    void addComponent(int entityId, ShapeType shape);

    void update(float fixedDeltaTime);
};
