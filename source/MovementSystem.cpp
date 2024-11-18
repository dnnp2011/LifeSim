#include "MovementSystem.h"
#include "ECSManager.h"
#include "Renderer.h"


MovementSystem::MovementSystem(std::mutex& physicsBufferMutex):
    m_renderer{ Container::Resolve<Renderer>().get() },
    m_physicsBufferMutex{ physicsBufferMutex },
    m_threadPool{ MAX_HARDWARE_THREADS }
{
    fprintf(stdout, "MovementSystem Instantiated\n");
}

void MovementSystem::Update(EntityData& physicsBufferWrite, float fixedDeltaTime)
{
    const auto& entities{ physicsBufferWrite.entities };
    auto& positions{ physicsBufferWrite.positions };
    const auto& velocities{ physicsBufferWrite.velocities };

    for (const auto& entity: entities) {
        IM_ASSERT(entity.id < positions.size() && entity.id < velocities.size());

        m_threadPool.enqueue(
            [this, &fixedDeltaTime, &positions, &velocities, &entity]() {
                auto& entityPosition{ positions[entity.id] };
                const auto& entityVelocity{ velocities[entity.id] };

                const std::lock_guard lock(m_physicsBufferMutex);

                entityPosition.x += (entityVelocity.dx * fixedDeltaTime * static_cast<float>(m_speed));
                entityPosition.y += (entityVelocity.dy * fixedDeltaTime * static_cast<float>(m_speed));
            }
        );

        #if 0
        Debounce(
            [entity, positions, velocities](const std::string& id) {
                const auto entityPosition{ positions.find(entity.id) };
                const auto entityVelocity{ velocities.find(entity.id) };

                fprintf(stdout, "%s---------------------------------\n", id.c_str());
                fprintf(stdout, "Entity ID: %d\n", entity.id);
                fprintf(stdout, "Position: (%.1f, %.1f)\n", entityPosition->second.x, entityPosition->second.y);
                fprintf(stdout, "Velocity: (%.1f, %.1f)\n", entityVelocity->second.dx, entityVelocity->second.dy);
            },
            5000,
            "MovementSystem::update::" + std::to_string(entity.id)
        );
        #endif
    }
}
