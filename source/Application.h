#pragma once

#include "Renderer.h"
#include "WorldManager.h"


class Application {
public:
    WorldManager worldManager;
    Renderer renderer;

public:
    Application();

    Application(const Application &) = delete;

    Application& operator=(const Application &) = delete;

    void Run();
};

inline Application g_Application;
