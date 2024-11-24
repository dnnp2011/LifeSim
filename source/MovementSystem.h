#pragma once

#include <Common.h>


class Renderer;

class MovementSystem {
private:
    Renderer* m_renderer;
    std::mutex* m_physicsBufferMutex;

    Threads::ThreadPool m_threadPool;
    uint8_t m_speed = 80;

public:
    explicit MovementSystem(std::mutex* physicsBufferMutex);

    explicit MovementSystem(const MovementSystem&) = delete;

    explicit MovementSystem(MovementSystem&&) = delete;

    void Update(EntityData& physicsBufferWrite, float fixedDeltaTime);
};
