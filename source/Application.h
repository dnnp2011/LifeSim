#pragma once

#include "ECSManager.h"
#include "Renderer.h"


class Application {
public:
    Renderer m_Renderer;
    ECSManager m_ECSManger;

private:
    Threads::ThreadPool m_threadPool{ std::thread::hardware_concurrency() };

public:
    Application();

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
