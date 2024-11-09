#include <iostream>
// #include <thread>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include "Application.h"


using Clock     = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration  = std::chrono::duration<float>;

Application::Application() {
    std::cout << "Starting LifeSim Application" << std::endl;
}

void Application::Run() {
    // std::thread entityThread(worldManager.run, std::ref(worldManager), 60.0);

    constexpr float fixedDeltaTime = 1.0f / 60.0f; // Fixed timestep (60 updates per second)
    float accumulator              = 0.0f;
    TimePoint previousTime         = Clock::now();

    while (!glfwWindowShouldClose(m_Renderer.window)) {
        glfwPollEvents();

        // Skip rendering if window is minimized
        if (glfwGetWindowAttrib(m_Renderer.window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);

            continue;
        }

        // Calculate elapsed time
        TimePoint currentTime = Clock::now();
        Duration frameTime    = currentTime - previousTime;
        previousTime          = currentTime;

        accumulator += frameTime.count();

        // Fixed timestep update loop
        while (accumulator >= fixedDeltaTime) {
            m_ECSManger.update(fixedDeltaTime); // TODO: Decouple WorldManager/ECSManager from Render Pipeline
            accumulator -= fixedDeltaTime;
        }

        m_Renderer.prerender();
        m_ECSManger.m_RenderSystem.update(
                m_ECSManger.m_Entities,
                m_ECSManger.m_Positions,
                m_ECSManger.m_Colliders,
                m_ECSManger.m_Shapes
                );
        m_Renderer.render();

        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // entityThread.join();
}
