#include <Random.h>
#include <ServiceContainer.h>

#include <GLFW/glfw3.h>

#include "CollisionSystem.h"
#include "MovementSystem.h"
#include "Renderer.h"

#include "ECSManager.h"


ECSManager::ECSManager()
{
    fprintf(stdout, "Instantiating ECSManager\n");

    m_MovementSystem  = Container::Bind<MovementSystem>(&m_PhysicsBufferMutex).get();
    m_CollisionSystem = Container::Bind<CollisionSystem>(&m_PhysicsBufferMutex).get();

    m_Entities.reserve(ENTITY_COUNT);
    m_Positions.reserve(ENTITY_COUNT);
    m_Velocities.reserve(ENTITY_COUNT);
    m_Colliders.reserve(ENTITY_COUNT);

    static int width, height;
    {
        const auto renderer = Container::Resolve<Renderer>();
        glfwGetWindowSize(renderer->m_Window, &width, &height);
    }

    for (size_t i = 0; i < ENTITY_COUNT; i++) {
        const unsigned int colliderSize{ Random<unsigned int>().generate(5, 15) };
        // ShapeType{ ShapeType{ Random<unsigned int>().generate(0, 3) } }; // Random Shape

        createEntity(
            Position{
                Position{
                    Random<float>().generate(0, static_cast<float>(width)),
                    Random<float>().generate(0, static_cast<float>(height))
                }
            },
            Velocity{ Velocity{ Random<float>().generate(-2, 2), Random<float>().generate(-2, 2) } },
            Collider{ Collider{ colliderSize, colliderSize } },
            ShapeType{ ShapeType::Rectangle },
            Color{ Random<float>().generate(0, 1), Random<float>().generate(0, 1), Random<float>().generate(0, 1), 1 }
        );
    }

    fprintf(stdout, "ECSManager Instantiated\n");
}

Entity ECSManager::createEntity(
    const Position& position,
    const Velocity& velocity,
    const Collider& collider,
    const ShapeType& shape,
    const Color& color
)
{
    static unsigned int nextId{ 0 };
    const Entity entity{ nextId++ };

    m_Entities.emplace_back(entity);
    m_PhysicsBufferWrite.entities[entity.id] = entity;

    addComponent(entity.id, position);
    addComponent(entity.id, velocity);
    addComponent(entity.id, collider);
    addComponent(entity.id, shape);
    addComponent(entity.id, color);

    return entity;
}

void ECSManager::addComponent(const unsigned int entityId, const Position& position)
{
    m_Positions[entityId]                    = position;
    m_PhysicsBufferWrite.positions[entityId] = position;
}

void ECSManager::addComponent(const unsigned int entityId, const Velocity& velocity)
{
    m_Velocities[entityId]                    = velocity;
    m_PhysicsBufferWrite.velocities[entityId] = velocity;
}

void ECSManager::addComponent(const unsigned int entityId, const Collider& collider)
{
    m_Colliders[entityId]                    = collider;
    m_PhysicsBufferWrite.colliders[entityId] = collider;
}

void ECSManager::addComponent(const unsigned int entityId, const ShapeType& shape)
{
    m_Shapes[entityId]                    = shape;
    m_PhysicsBufferWrite.shapes[entityId] = shape;
}

void ECSManager::addComponent(const unsigned int entityId, const Color& color)
{
    m_PhysicsBufferWrite.colors[entityId] = color;
}

void ECSManager::Update(const float fixedDeltaTime)
{
    m_CollisionSystem->Update(m_PhysicsBufferWrite);
    m_MovementSystem->Update(m_PhysicsBufferWrite, fixedDeltaTime);

    SwapBuffers();
}

void ECSManager::SwapBuffers()
{
    m_PhysicsBufferRead = m_PhysicsBufferWrite;
    m_RenderBuffer      = m_PhysicsBufferWrite;
}
