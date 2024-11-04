#pragma once


#include <iostream>
#include <vector>
#include <unordered_map>

#include "MovementSystem.h"

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Collider {
    int width, height;
};

struct Entity {
    int id;
};


class ECSManager {
public:
    std::vector<Entity> entities;
    std::unordered_map<int, Position> positions;
    std::unordered_map<int, Velocity> velocities;
    std::unordered_map<int, Collider> colliders;

    MovementSystem movementSystem;
    CollisionSystem collisionSystem;

    ECSManager() {
        // TODO: Instantiate Entities, Components, and Systems
        // std::vector<Entity> entities                 = { { 1 }, { 2 }, { 3 }, { 4 } };
        // std::unordered_map<int, Position> positions  = { { 1, { 0, 0 } }, { 2, { 10, 10 } }, { 3, { 20, 20 } }, { 4, { 30, 30 } } };
        // std::unordered_map<int, Velocity> velocities = { { 1, { 1, 1 } }, { 2, { 0, -1 } }, { 3, { 0.5, 0.5 } }, { 4, { -1, -1 } } };

        for (int i = 0; i < 5; i++) {
            createEntity();
        }
    }

    Entity createEntity() {
        static int nextId = 0;
        const Entity entity{ nextId++ };

        entities.push_back(entity);

        addComponent(entity.id, Position{ 0, 0 });
        addComponent(entity.id, Velocity{ 1, 1 });
        // TODO: Grab cellSize from WorldManager (need to figure out how to make Singletons and Global variables available to all classes)
        addComponent(entity.id, Collider{ 10, 10 });

        return entity;
    }

    void addComponent(int entityId, Position position) {
        positions[entityId] = position;
    }

    void addComponent(int entityId, Velocity velocity) {
        velocities[entityId] = velocity;
    }

    void addComponent(int entityId, Collider collider) {
        colliders[entityId] = collider;
    }

    void update() {
        // Run collision detection first
        collisionSystem.update(entities, positions, colliders);

        // Run movement system with the collision information
        movementSystem.update(entities, positions, velocities, collisionSystem.entitiesToStop);

        for (const auto &entity: entities) {
            std::cout << "Entity " << entity.id << " Position: (" << positions[entity.id].x << ", " << positions[entity.id].y << ")\n";
            std::cout << "Entity " << entity.id << " Velocity: (" << velocities[entity.id].dx << ", " << velocities[entity.id].dy << ")\n";
            std::cout << "Entity " << entity.id << " Collider: (" << colliders[entity.id].width << ", " << colliders[entity.id].height << ")\n";
            std::cout << std::endl;
        }
    }
};
