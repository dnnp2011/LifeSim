#include <iostream>

#include "ECSManager.h"
#include "../utils/Random.h"

#define ENTITY_COUNT 3

ECSManager::ECSManager() {
    entities.reserve(ENTITY_COUNT);
    positions.reserve(ENTITY_COUNT);
    velocities.reserve(ENTITY_COUNT);
    colliders.reserve(ENTITY_COUNT);

    for (size_t i = 0; i < ENTITY_COUNT; i++) {
        int colliderSize;

        createEntity(
            Position{ Position{ Random<float>().generate(0, 1920), Random<float>().generate(0, 1080) } },
            Velocity{ Velocity{ Random<float>().generate(-1, 1), Random<float>().generate(-1, 1) } },
            Collider{ Collider{ colliderSize = Random<int>().generate(3, 50), colliderSize } },
            ShapeType{ ShapeType{ Random<uint_fast8_t>().generate(0, 3) } }
        );
    }
}

Entity ECSManager::createEntity(Position position, Velocity velocity, Collider collider, ShapeType shape) {
    static int nextId = 0;
    const Entity entity{ nextId++ };

    entities.push_back(entity);

    addComponent(entity.id, position);
    addComponent(entity.id, velocity);
    addComponent(entity.id, collider);
    addComponent(entity.id, shape);

    return entity;
}

void ECSManager::addComponent(const int entityId, const Position position) {
    positions[entityId] = position;
}

void ECSManager::addComponent(const int entityId, const Velocity velocity) {
    velocities[entityId] = velocity;
}

void ECSManager::addComponent(const int entityId, const Collider collider) {
    colliders[entityId] = collider;
}

void ECSManager::addComponent(const int entityId, const ShapeType shape) {
    shapes[entityId] = shape;
}

void ECSManager::update() {
    collisionSystem.update(entities, positions, colliders);
    movementSystem.update(entities, positions, velocities, collisionSystem.entitiesToStop);
    renderSystem.update(entities, positions, colliders, shapes); // Maybe this should be called at same level of GUI Renderer
}
