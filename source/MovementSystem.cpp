#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "MovementSystem.h"
#include "Common.h"

static std::mutex mtx;

void MovementSystem::update(
    const float fixedDeltaTime,
    const EntityBuffer& entities,
    PositionBuffer& positions,
    VelocityBuffer& velocities,
    const std::unordered_set<int>& entitiesToStop,
    const std::unordered_set<int>& entitiesOutOfBounds
) const {
    ThreadBuffer threads;

    // TODO: Don't create more threads than the number of cores (what about hyperthreading?)
    // const auto maxThreads = std::thread::hardware_concurrency();

    for (const auto& entity: entities) {
        Debounce(
            [entity, positions, velocities](const std::string& id) {
                const auto entityPosition{ positions.find(entity.id) };
                const auto entityVelocity{ velocities.find(entity.id) };

                std::cout << id << "---------------------------------" << "\n";
                std::cout << "Entity ID: " << entity.id << "\n";
                std::cout << "Position: (" << entityPosition->second.x << ", " << entityPosition->second.y << ")" << "\n";
                std::cout << "Velocity: (" << entityVelocity->second.dx << ", " << entityVelocity->second.dy << ")" << std::endl;
            },
            5000,
            "MovementSystem::update::" + std::to_string(entity.id)
        );

        // if (entitiesOutOfBounds.contains(entity.id)) {
        // velocities[entity.id].dx *= -1;
        // velocities[entity.id].dy *= -1;
        // }

        //if (!entitiesToStop.contains(entity.id)) {
        threads.emplace_back(
            [this, &fixedDeltaTime, &positions, &velocities, &entity]() {
                const auto entityPosition{ positions.find(entity.id) };
                const auto entityVelocity{ velocities.find(entity.id) };

                if (entityPosition != positions.end() && entityVelocity != velocities.end()) {
                    const std::lock_guard lock(mtx);

                    positions[entity.id].x += (entityVelocity->second.dx * fixedDeltaTime * static_cast<float>(m_speed));
                    positions[entity.id].y += (entityVelocity->second.dy * fixedDeltaTime * static_cast<float>(m_speed));
                }
            }
        );
        //}
    }

    for (auto& thread: threads) {
        thread.join();
    }
}
