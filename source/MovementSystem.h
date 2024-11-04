#pragma once

#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common.h"

class MovementSystem {
public:
    void update(std::vector<Entity> &entities, std::unordered_map<int, ImVec2> &positions, std::unordered_map<int, Velocity> &velocities, const std::unordered_set<int> &entitiesToStop) const {
        std::vector<std::thread> threads;

        for (const auto &entity: entities) {
            if (entitiesToStop.find(entity.id) == entitiesToStop.end()) {
                threads.emplace_back([this, &positions, &velocities, &entity]() {
                    auto entityPosition = positions.find(entity.id);
                    auto entityVelocity = velocities.find(entity.id);

                    if (entityPosition != positions.end() && entityVelocity != velocities.end()) {
                        positions[entity.id].x += entityVelocity->second.dx;
                        positions[entity.id].y += entityVelocity->second.dy;
                    }
                });
            }
        }

        for (auto &thread: threads) {
            thread.join();
        }
    }
};


class CollisionSystem {
public:
    std::unordered_set<int> entitiesToStop;

    void update(std::vector<Entity> &entities, std::unordered_map<int, ImVec2> &positions, std::unordered_map<int, Collider> &colliders) {
        entitiesToStop.clear();

        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                auto &entityA = entities[i];
                auto &entityB = entities[j];

                if (positions.find(entityA.id) != positions.end() && colliders.find(entityA.id) != colliders.end() && positions.find(entityB.id) != positions.end() && colliders.find(entityB.id) != colliders.end()) {
                    if (isColliding(positions[entityA.id], colliders[entityA.id], positions[entityB.id], colliders[entityB.id])) {
                        entitiesToStop.insert(entityA.id);
                        entitiesToStop.insert(entityB.id);
                    }
                }
            }
        }
    }

private:
    static bool isColliding(const ImVec2 &posA, const Collider &colA, const ImVec2 &posB, const Collider &colB) {
        return !(posA.x + colA.width < posB.x || posA.x >
                 posB.x + colB.width || posA.y + colA.height < posB.y || posA.y >
                 posB.y + colB.height
        );
    }
};
