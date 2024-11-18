#pragma once

#include <Common.h>


class MovementSystem;
class CollisionSystem;

class ECSManager {
public:
    EntityData m_RenderBuffer{};
    EntityData m_PhysicsBufferRead{};
    EntityData m_PhysicsBufferWrite{};

    EntityBuffer m_Entities;
    PositionBuffer m_Positions;
    VelocityBuffer m_Velocities;
    ColliderBuffer m_Colliders;
    ShapeTypeBuffer m_Shapes;

    std::mutex m_PhysicsBufferMutex;

private:
    MovementSystem* m_MovementSystem;
    CollisionSystem* m_CollisionSystem;

public:
    ECSManager();

    void Update(float fixedDeltaTime);

    void SwapBuffers();

private:
    Entity createEntity(
        const Position& position,
        const Velocity& velocity,
        const Collider& collider,
        const ShapeType& shape,
        const Color& color
    );

    void addComponent(int unsigned entityId, const Position& position);

    void addComponent(int unsigned entityId, const Velocity& velocity);

    void addComponent(int unsigned entityId, const Collider& collider);

    void addComponent(int unsigned entityId, const ShapeType& shape);

    void addComponent(int unsigned entityId, const Color& color);
};
