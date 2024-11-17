#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <GLFW/glfw3.h>

#include "CollisionSystem.h"
#include "Application.h"


void CollisionSystem::update(
    const EntityBuffer& entities,
    PositionBuffer& positions,
    VelocityBuffer& velocities,
    ColliderBuffer& colliders
)
{
    static int width, height;
    glfwGetWindowSize(Container::Resolve<Renderer>()->m_Window, &width, &height);

    for (const auto& entityA: entities) {
        if (isOutOfBounds(positions[entityA.id], colliders[entityA.id], velocities[entityA.id], width, height)) {
            m_threadPool.enqueue(
                [this, &positions, &colliders, &entityA, &velocities]() {
                    const std::lock_guard oobLock(m_oobMtx);

                    ImVec2 boundaryNormal{ 1, 0 };
                    if (IsGreaterThanOrEqual<float>(positions[entityA.id].x + static_cast<float>(colliders[entityA.id].width), static_cast<float>(width))) {
                        boundaryNormal = { -1, 0 };
                    } else if (IsLessThanOrEqual<float>(positions[entityA.id].y, 0)) {
                        boundaryNormal = { 0, 1 };
                    } else if (IsGreaterThanOrEqual<float>(positions[entityA.id].y + static_cast<float>(colliders[entityA.id].height), static_cast<float>(height))) {
                        boundaryNormal = { 0, -1 };
                    }

                    velocities[entityA.id] = static_cast<Velocity>(ImMath::Reflect((ImVec2)velocities[entityA.id], boundaryNormal));
                }
            );

            continue;
        }

        for (const auto& entityB: entities) {
            if (entityA.id == entityB.id)
                continue;

            m_threadPool.enqueue(
                [this, &positions, &colliders, &entityA, &entityB, &velocities]() {
                    const auto entityAPosition{ positions.find(entityA.id) };
                    const auto entityACollider{ colliders.find(entityA.id) };
                    const auto entityBPosition{ positions.find(entityB.id) };
                    const auto entityBCollider{ colliders.find(entityB.id) };

                    if (entityAPosition != positions.end() && entityACollider != colliders.end() && entityBPosition != positions.end() && entityBCollider != colliders.end()) {
                        if (isColliding(positions[entityA.id], colliders[entityA.id], positions[entityB.id], colliders[entityB.id])) {
                            const std::lock_guard lock(m_mtx);

                            ImVec2 normal = ImMath::CalculateNormal(
                                ImVec2{ positions[entityA.id].x, positions[entityA.id].y },
                                ImVec2{ positions[entityB.id].x, positions[entityB.id].y }
                            );

                            velocities[entityA.id] = static_cast<Velocity>(ImMath::Reflect((ImVec2)velocities[entityA.id], normal));
                            velocities[entityB.id] = static_cast<Velocity>(ImMath::Reflect((ImVec2)velocities[entityB.id], ImMath::Invert(normal)));

                            // const float penetrationDepth = ImMath::CalculatePenetrationDepth(
                            //     static_cast<const ImVec2&>(positions[entityA.id]),
                            //     static_cast<const ImVec2&>(positions[entityB.id]),
                            //     colliders[entityA.id],
                            //     colliders[entityB.id]
                            // );
                            // const float penetrationDepth = ImMath::Distance(
                            //     static_cast<const ImVec2&>(positions[entityA.id]),
                            //     static_cast<const ImVec2&>(positions[entityB.id])
                            // ) - ((colliders[entityA.id].width + colliders[entityB.id].width) / 2);
                        }
                    }
                }
            );
        }
    }
}

bool CollisionSystem::isColliding(
    const Position& posA,
    const Collider& colA,
    const Position& posB,
    const Collider& colB
)
{
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
)
{
    // TODO: Improve to predict if entity will be out of bounds in the next frame
    return IsLessThanOrEqual<float>(pos.x, 0)
            || IsGreaterThanOrEqual<float>(pos.x + static_cast<float>(col.width), static_cast<float>(width))
            || IsLessThanOrEqual<float>(pos.y, 0)
            || IsGreaterThanOrEqual(pos.y + static_cast<float>(col.height), static_cast<float>(height));
}
