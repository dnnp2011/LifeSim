#pragma once

#include <iostream>
#include <ServiceContainer.h>

#include "ECSManager.h"
#include "Renderer.h"


class Application {
public:
    Renderer* m_Renderer;
    ECSManager m_ECSManger;

private:
    std::thread m_physicsThread;
    std::thread m_renderThread;
    std::atomic<bool> m_running{ true };
    Threads::ThreadPool m_threadPool{ std::thread::hardware_concurrency() };
    float m_maxZoom{ 10.0f };
    float m_minZoom{ 0.1f };

public:
    Application(): m_Renderer{ (Container::Bind<Renderer>().get()) } {
        std::cout << "Booting Application Instance" << std::endl;
    }

    Application(const Application&) = delete;

    Application& operator=(const Application&) = delete;

    static void DrawPerformanceGui(
        float frameTime,
        float fps,
        double windowsFrameTime,
        double windowsFps,
        float physicsFrameTime,
        float physicsStepsPerSecond,
        float cpuUsage
    );

    void Run();
};

inline Application g_Application;
