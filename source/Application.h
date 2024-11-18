#pragma once

#include <Common.h>


class Instrumentation;
class Renderer;
class InputManager;
class ECSManager;

class Application {
public:
    Instrumentation* m_Instrumentation;
    Renderer* m_Renderer;
    ECSManager* m_ECSManager;
    InputManager* m_InputManager;

private:
    std::thread m_physicsThread;
    std::thread m_renderThread;
    std::atomic<bool> m_running{ true };

public:
    explicit Application();

    Application(const Application&) = delete;

    Application& operator=(const Application&) = delete;

    void Run() const;
};
