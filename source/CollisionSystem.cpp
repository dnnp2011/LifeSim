#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CollisionSystem.h"


static std::mutex mtx;

void CollisionSystem::update(
    const EntityBuffer& entities,
    PositionBuffer& positions,
    ColliderBuffer& colliders
) {
    entitiesToStop.clear();

    ThreadBuffer threads;

    for (const auto& entityA: entities) {
        for (const auto& entityB: entities) {
            if (entityA.id == entityB.id)
                continue;

            threads.emplace_back(
                [this, &positions, &colliders, &entityA, &entityB]() {
                    const auto entityAPosition = positions.find(entityA.id);
                    const auto entityACollider = colliders.find(entityA.id);
                    const auto entityBPosition = positions.find(entityB.id);
                    const auto entityBCollider = colliders.find(entityB.id);

                    if (entityAPosition != positions.end() && entityACollider != colliders.end() && entityBPosition != positions.end() && entityBCollider != colliders.end()) {
                        if (isColliding(positions[entityA.id], colliders[entityA.id], positions[entityB.id], colliders[entityB.id])) {
                            const std::lock_guard lock(mtx);

                            entitiesToStop.insert(entityA.id);
                            entitiesToStop.insert(entityB.id);
                        }
                    }
                }
            );
        }
    }

    for (auto& thread: threads) {
        thread.join();
    }
}

bool CollisionSystem::isColliding(
    const Position& posA,
    const Collider& colA,
    const Position& posB,
    const Collider& colB
) {
    return !(posA.x + static_cast<float>(colA.width) < posB.x
        || posA.x > posB.x + static_cast<float>(colB.width)
        || posA.y + static_cast<float>(colA.height) < posB.y
        || posA.y > posB.y + static_cast<float>(colB.height));
}
