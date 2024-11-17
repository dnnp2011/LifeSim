#pragma once

#include <Instrumentation.h>
#include <iostream>
#include <ServiceContainer.h>

#include "ECSManager.h"
#include "InputManager.h"
#include "Renderer.h"


class Application
{
public:
    Instrumentation* m_Instrumentation;
    Renderer* m_Renderer;
    InputManager* m_InputManager;
    ECSManager* m_ECSManager;

private:
    std::thread m_physicsThread;
    std::thread m_renderThread;
    std::atomic<bool> m_running{ true };

public:
    Application():
        m_Instrumentation{ (Container::Bind<Instrumentation>().get()) },
        m_Renderer{ (Container::Bind<Renderer>().get()) },
        m_InputManager{ (Container::Bind<InputManager>().get()) },
        m_ECSManager{ (Container::Bind<ECSManager>().get()) }
    {
        std::cout << "Booting Application Instance" << std::endl;
    }

    Application(const Application&) = delete;

    Application& operator=(const Application&) = delete;

    void Run();
};

inline Application g_Application;
