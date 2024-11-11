#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "MovementSystem.h"
#include "Common.h"


void MovementSystem::update(
    const float fixedDeltaTime,
    const EntityBuffer& entities,
    PositionBuffer& positions,
    const VelocityBuffer& velocities,
    const std::unordered_set<int>& entitiesToStop
) const {
    ThreadBuffer threads;

    // TODO: Don't create more threads than the number of cores (what about hyperthreading?)
    // const auto maxThreads = std::thread::hardware_concurrency();

    for (const auto& entity: entities) {
        if (!entitiesToStop.contains(entity.id)) {
            threads.emplace_back(
                [this, &fixedDeltaTime, &positions, &velocities, &entity]() {
                    const auto entityPosition = positions.find(entity.id);
                    const auto entityVelocity = velocities.find(entity.id);

                    if (entityPosition != positions.end() && entityVelocity != velocities.end()) {
                        positions[entity.id].x += entityVelocity->second.dx * fixedDeltaTime * static_cast<float>(m_speed);
                        positions[entity.id].y += entityVelocity->second.dy * fixedDeltaTime * static_cast<float>(m_speed);
                    }
                }
            );
        }
    }

    for (auto& thread: threads) {
        thread.join();
    }
}
