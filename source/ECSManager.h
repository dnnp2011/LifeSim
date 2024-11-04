#pragma once


#include <iostream>
#include <vector>
#include <unordered_map>

#include "common.h"
#include "Renderer.h"
#include "MovementSystem.h"
#include "RenderSystem.h"

class ECSManager {
public:
    std::vector<Entity> entities;
    std::unordered_map<int, ImVec2> positions;
    std::unordered_map<int, Velocity> velocities;
    std::unordered_map<int, Collider> colliders;
    std::unordered_map<int, ShapeType> shapes;

    MovementSystem movementSystem;
    CollisionSystem collisionSystem;
    RenderSystem renderSystem;

    ECSManager() {
        for (int i = 0; i < 5; i++) {
            Entity entity = createEntity();

            std::cout << "Entity " << entity.id << " Created\n";
            std::cout << "Entity " << entity.id << " Position: (" << positions[entity.id].x << ", " << positions[entity.id].y << ")" << std::endl;
        }
    }

    Entity createEntity() {
        static int nextId = 0;
        const Entity entity{ nextId++ };

        entities.push_back(entity);

        addComponent(entity.id, ImVec2{ static_cast<float>(0 + rand() % 1920), static_cast<float>(0 + rand() % 1080) });
        addComponent(entity.id, Velocity{ static_cast<float>((-1 + rand() % 2) - 1), static_cast<float>((-1 + rand() % 2) - 1) });
        // TODO: Grab cellSize from WorldManager (need to figure out how to make Singletons and Global variables available to all classes)
        addComponent(entity.id, Collider{ 10, 10 });
        addComponent(entity.id, static_cast<ShapeType>(rand() % 3)); // TODO: Improve Shape selection

        return entity;
    }

    void addComponent(const int entityId, const ImVec2 position) {
        positions[entityId] = position;
    }

    void addComponent(const int entityId, const Velocity velocity) {
        velocities[entityId] = velocity;
    }

    void addComponent(const int entityId, const Collider collider) {
        colliders[entityId] = collider;
    }

    void addComponent(const int entityId, const ShapeType shape) {
        shapes[entityId] = shape;
    }

    void update() {
        std::cout << "Running ECSManager::update()" << std::endl;

        // Run collision detection first
        collisionSystem.update(entities, positions, colliders);

        // Run movement system with the collision information
        movementSystem.update(entities, positions, velocities, collisionSystem.entitiesToStop);

        // Run RenderSystem
        renderSystem.update(entities, positions, shapes);

        // for (const auto &entity: entities) {
        //     std::cout << "Entity " << entity.id << " Position: (" << positions[entity.id].x << ", " << positions[entity.id].y << ")\n";
        //     std::cout << "Entity " << entity.id << " Velocity: (" << velocities[entity.id].dx << ", " << velocities[entity.id].dy << ")\n";
        //     std::cout << "Entity " << entity.id << " Collider: (" << colliders[entity.id].width << ", " << colliders[entity.id].height << ")\n";
        //     std::cout << std::endl;
        // }
    }
};
