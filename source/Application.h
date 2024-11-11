#pragma once

#include "ECSManager.h"
#include "Renderer.h"
#include "WorldManager.h"


class Application {
public:
    WorldManager m_WorldManager;
    Renderer m_Renderer;
    ECSManager m_ECSManger;

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
