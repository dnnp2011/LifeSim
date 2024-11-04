#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "MovementSystem.h"
#include "common.h"


void MovementSystem::update(std::vector<Entity> &entities, std::unordered_map<int, ImVec2> &positions, std::unordered_map<int, Velocity> &velocities, const std::unordered_set<int> &entitiesToStop) const {
    std::vector<std::thread> threads;

    for (const auto &entity: entities) {
        if (entitiesToStop.find(entity.id) == entitiesToStop.end()) {
            threads.emplace_back([this, &positions, &velocities, &entity]() {
                const auto entityPosition = positions.find(entity.id);
                const auto entityVelocity = velocities.find(entity.id);

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
