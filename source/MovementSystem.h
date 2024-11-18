#pragma once

#include <Common.h>
#include <ServiceContainer.h>


class Renderer;

class MovementSystem {
private:
    Renderer* m_renderer;
    std::mutex& m_physicsBufferMutex;

    Threads::ThreadPool m_threadPool;
    uint8_t m_speed = 80;

public:
    explicit MovementSystem(std::mutex& physicsBufferMutex);

    explicit MovementSystem(const MovementSystem&) = default;

    explicit MovementSystem(MovementSystem&&) = default;

    void Update(EntityData& physicsBufferWrite, float fixedDeltaTime);
};
