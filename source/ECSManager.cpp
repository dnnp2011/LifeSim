#include <iostream>

#include "ECSManager.h"
#include "../utils/Random.h"


ECSManager::ECSManager() {
    m_Entities.reserve(ENTITY_COUNT);
    m_Positions.reserve(ENTITY_COUNT);
    m_Velocities.reserve(ENTITY_COUNT);
    m_Colliders.reserve(ENTITY_COUNT);

    for (size_t i = 0; i < ENTITY_COUNT; i++) {
        int colliderSize;

        createEntity(
            Position{ Position{ Random<float>().generate(0, 1920), Random<float>().generate(0, 1080) } },
            Velocity{ Velocity{ Random<float>().generate(-1, 1), Random<float>().generate(-1, 1) } },
            Collider{ Collider{ colliderSize = Random<int>().generate(15, 100), colliderSize } },
            ShapeType{ ShapeType::Rectangle }
            // ShapeType{ ShapeType{ Random<unsigned int>().generate(0, 3) } }
        );
    }
}

Entity ECSManager::createEntity(const Position& position, const Velocity& velocity, const Collider& collider, const ShapeType& shape) {
    static int nextId = 0;
    const Entity entity{ nextId++ };

    m_Entities.emplace_back(entity);

    addComponent(entity.id, position);
    addComponent(entity.id, velocity);
    addComponent(entity.id, collider);
    addComponent(entity.id, shape);

    return entity;
}

void ECSManager::addComponent(const int entityId, const Position& position) {
    m_Positions[entityId] = position;
}

void ECSManager::addComponent(const int entityId, const Velocity& velocity) {
    m_Velocities[entityId] = velocity;
}

void ECSManager::addComponent(const int entityId, const Collider& collider) {
    m_Colliders[entityId] = collider;
}

void ECSManager::addComponent(const int entityId, const ShapeType& shape) {
    m_Shapes[entityId] = shape;
}

void ECSManager::update(const float fixedDeltaTime) {
    m_CollisionSystem.update(m_Entities, m_Positions, m_Colliders);
    m_MovementSystem.update(fixedDeltaTime, m_Entities, m_Positions, m_Velocities, m_CollisionSystem.entitiesToStop);
}
