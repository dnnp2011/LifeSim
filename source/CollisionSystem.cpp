#include <mutex>
#include <ServiceContainer.h>
#include <unordered_set>
#include <vector>

#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "CollisionSystem.h"


CollisionSystem::CollisionSystem(std::mutex* physicsBufferMutex):
    m_renderer{ Container::Resolve<Renderer>().get() },
    m_physicsBufferMutex(physicsBufferMutex),
    m_threadPool{ MAX_HARDWARE_THREADS }
{
    if (physicsBufferMutex == nullptr)
        throw std::invalid_argument("CollisionSystem(): physicsBufferMutex == nullptr");
    else
        fprintf(
            stdout,
            "CollisionSystem: Define physicsBufferMutex@: %p\n",
            static_cast<const void* const>(m_physicsBufferMutex)
        );

    fprintf(stdout, "CollisionSystem Instantiated\n");
}


void CollisionSystem::Update(EntityData& physicsBufferWrite)
{
    const auto& entities{ physicsBufferWrite.entities };
    const auto& positions{ physicsBufferWrite.positions };
    const auto& colliders{ physicsBufferWrite.colliders };
    auto& velocities{ physicsBufferWrite.velocities };

    for (const auto& entityA: entities) {
        IM_ASSERT(entityA.id < positions.size() && entityA.id < colliders.size() && entityA.id < velocities.size());

        if (isOutOfBounds(positions[entityA.id], colliders[entityA.id], velocities[entityA.id])) {
            m_threadPool.enqueue(
                [this, &positions, &colliders, &entityA, &velocities] {
                    ImVec2 boundaryNormal{ 1, 0 };
                    if (ImMath::IsGreaterThanOrEqual<float>(
                        positions[entityA.id].x + static_cast<float>(colliders[entityA.id].width),
                        static_cast<float>(m_renderer->m_BufferWidth)
                    )) {
                        boundaryNormal = { -1, 0 };
                    }
                    else if (ImMath::IsLessThanOrEqual<float>(positions[entityA.id].y, 0)) {
                        boundaryNormal = { 0, 1 };
                    }
                    else if (ImMath::IsGreaterThanOrEqual<float>(
                        positions[entityA.id].y + static_cast<float>(colliders[entityA.id].height),
                        static_cast<float>(m_renderer->m_BufferHeight)
                    )) {
                        boundaryNormal = { 0, -1 };
                    }

                    {
                        if (m_physicsBufferMutex == nullptr)
                            // Why is the mutex becoming 0xfff... instead of 0x000... (nullptr)?
                            throw std::runtime_error("CollisionSystem: physicsBufferMutex == nullptr");
                        else
                            fprintf(
                                stdout,
                                "CollisionSystem: physicsBufferMutex address: %p\n",
                                static_cast<const void* const>(m_physicsBufferMutex)
                            );

                        const std::lock_guard lock(*m_physicsBufferMutex);
                        velocities[entityA.id] = static_cast<Velocity>(ImMath::Reflect(
                            (ImVec2)velocities[entityA.id],
                            boundaryNormal
                        ));
                    }
                }
            );

            continue;
        }

        for (const auto& entityB: entities) {
            IM_ASSERT(entityB.id < positions.size() && entityB.id < colliders.size() && entityB.id < velocities.size());

            if (entityA.id == entityB.id)
                continue;

            m_threadPool.enqueue(
                [this, &positions, &colliders, &entityA, &entityB, &velocities] {
                    const auto& entityAPosition{ positions[entityA.id] };
                    const auto& entityACollider{ colliders[entityA.id] };
                    const auto& entityBPosition{ positions[entityB.id] };
                    const auto& entityBCollider{ colliders[entityB.id] };

                    if (isColliding(entityAPosition, entityACollider, entityBPosition, entityBCollider)) {
                        ImVec2 normal = ImMath::CalculateNormal(
                            ImVec2{ entityAPosition.x, entityAPosition.y },
                            ImVec2{ entityBPosition.x, entityBPosition.y }
                        );

                        if (m_physicsBufferMutex == nullptr)
                            throw std::runtime_error("CollisionSystem: physicsBufferMutex == nullptr");
                        else
                            fprintf(stdout, "CollisionSystem(1): physicsBufferMutex appears valid\n");

                        const std::lock_guard lock(*m_physicsBufferMutex);
                        velocities[entityA.id] = static_cast<Velocity>(ImMath::Reflect(
                            (ImVec2)velocities[entityA.id],
                            normal
                        ));
                        velocities[entityB.id] = static_cast<Velocity>(ImMath::Reflect(
                            (ImVec2)velocities[entityB.id],
                            ImMath::Invert(normal)
                        ));
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
) const
{
    return ImMath::IsLessThanOrEqual<float>(posA.x, posB.x + static_cast<float>(colB.width))
            && ImMath::IsGreaterThanOrEqual<float>(posA.x + static_cast<float>(colA.width), posB.x)
            && ImMath::IsLessThanOrEqual<float>(posA.y, posB.y + static_cast<float>(colB.height))
            && ImMath::IsGreaterThanOrEqual<float>(posA.y + static_cast<float>(colA.height), posB.y);
}

bool CollisionSystem::isOutOfBounds(
    const Position& pos,
    const Collider& col,
    const Velocity& vel
) const
{
    // TODO: Improve to predict if entity will be out of bounds in the next frame
    return ImMath::IsLessThanOrEqual<float>(pos.x, 0)
            || ImMath::IsGreaterThanOrEqual<float>(
                pos.x + static_cast<float>(col.width),
                static_cast<float>(m_renderer->m_BufferWidth)
            )
            || ImMath::IsLessThanOrEqual<float>(pos.y, 0)
            || ImMath::IsGreaterThanOrEqual(
                pos.y + static_cast<float>(col.height),
                static_cast<float>(m_renderer->m_BufferHeight)
            );
}
