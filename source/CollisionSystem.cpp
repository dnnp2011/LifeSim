#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <GLFW/glfw3.h>

#include "CollisionSystem.h"
#include "Application.h"


static std::mutex mtx;
static std::mutex oobMtx;

void CollisionSystem::update(
    const EntityBuffer& entities,
    PositionBuffer& positions,
    VelocityBuffer& velocities,
    ColliderBuffer& colliders
) {
    // entitiesToStop.clear();

    ThreadBuffer threads;

    static int width, height;
    glfwGetWindowSize(g_Application.m_Renderer.window, &width, &height);

    for (const auto& entityA: entities) {
        if (isOutOfBounds(positions[entityA.id], colliders[entityA.id], velocities[entityA.id], width, height)) {
            const std::lock_guard oobLock(oobMtx);

            // TODO: Refactor this to use a boundary enum
            // TODO: Return boundary normal from isOutOfBounds
            ImVec2 boundaryNormal{ 1, 0 }; // Default to left viewport boundary

            if (IsGreaterThanOrEqual<float>(positions[entityA.id].x + static_cast<float>(colliders[entityA.id].width), static_cast<float>(width))) {
                boundaryNormal = { -1, 0 };
            } else if (IsLessThanOrEqual<float>(positions[entityA.id].y, 0)) {
                boundaryNormal = { 0, 1 };
            } else if (IsGreaterThanOrEqual<float>(positions[entityA.id].y + static_cast<float>(colliders[entityA.id].height), static_cast<float>(height))) {
                boundaryNormal = { 0, -1 };
            }

            velocities[entityA.id] = static_cast<Velocity>(ImMath::Reflect((ImVec2)velocities[entityA.id], boundaryNormal));

            // entitiesOutOfBounds.insert(entityA.id);

            continue; // Skip collision detection for out of bounds entities
        }

        for (const auto& entityB: entities) {
            if (entityA.id == entityB.id)
                continue;

            threads.emplace_back(
                [this, &positions, &colliders, &entityA, &entityB, &velocities]() {
                    const auto entityAPosition{ positions.find(entityA.id) };
                    const auto entityACollider{ colliders.find(entityA.id) };
                    const auto entityBPosition{ positions.find(entityB.id) };
                    const auto entityBCollider{ colliders.find(entityB.id) };

                    if (entityAPosition != positions.end() && entityACollider != colliders.end() && entityBPosition != positions.end() && entityBCollider != colliders.end()) {
                        if (isColliding(positions[entityA.id], colliders[entityA.id], positions[entityB.id], colliders[entityB.id])) {
                            const std::lock_guard lock(mtx);

                            velocities[entityA.id].dx *= -1;
                            velocities[entityA.id].dy *= -1;

                            velocities[entityB.id].dx *= -1;
                            velocities[entityB.id].dy *= -1;

                            // entitiesToStop.insert(entityA.id);
                            // entitiesToStop.insert(entityB.id);
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
    return IsLessThanOrEqual<float>(posA.x, posB.x + static_cast<float>(colB.width))
            && IsGreaterThanOrEqual<float>(posA.x + static_cast<float>(colA.width), posB.x)
            && IsLessThanOrEqual<float>(posA.y, posB.y + static_cast<float>(colB.height))
            && IsGreaterThanOrEqual<float>(posA.y + static_cast<float>(colA.height), posB.y);
}

bool CollisionSystem::isOutOfBounds(
    const Position& pos,
    const Collider& col,
    const Velocity& vel,
    const int width,
    const int height
) {
    return IsLessThanOrEqual<float>(pos.x, 0)
            || IsGreaterThanOrEqual<float>(pos.x + static_cast<float>(col.width), static_cast<float>(width))
            || IsLessThanOrEqual<float>(pos.y, 0)
            || IsGreaterThanOrEqual(pos.y + static_cast<float>(col.height), static_cast<float>(height));
}
