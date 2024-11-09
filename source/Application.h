#pragma once

#include "Renderer.h"
#include "WorldManager.h"
#include "ECSManager.h"


class Application {
public:
    WorldManager m_WorldManager;
    Renderer m_Renderer;
    ECSManager m_ECSManger;

public:
    Application();

    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    void Run();
};

inline Application g_Application;
