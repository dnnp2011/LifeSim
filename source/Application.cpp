#include <chrono>
#include <imgui_impl_glfw.h>
#include <iostream>
#include <windows.h>
#include <GLFW/glfw3.h>

#include "Application.h"


static void window_refresh_callback(GLFWwindow* window) {
    // NOTE: This is a workaround for the issue where the window doesn't refresh when minimized
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
        glfwRestoreWindow(window);
    }
}

void Application::Run() {
    static constexpr auto fixedDeltaTime{ 1.0f / 30.0f }; // Fixed timestep (30 updates per second)
    static float physicsAccumulator{ 0.0f };

    m_Instrumentation->Start();

    glfwSetWindowRefreshCallback(m_Renderer->m_Window, window_refresh_callback);

    while (!glfwWindowShouldClose(m_Renderer->m_Window)) {
        if (m_InputManager->Poll() < 0)
            continue;

        m_Instrumentation->Measure();

        /* region Physics Loop */
        physicsAccumulator += m_Instrumentation->GetFrameTime();

        // -- Fixed Timestep Loop -- //
        while (physicsAccumulator >= fixedDeltaTime) {
            m_ECSManager->update(fixedDeltaTime);

            physicsAccumulator -= fixedDeltaTime;

            m_Instrumentation->MeasurePhysics();
        }
        /* endregion */

        m_Renderer->NewFrame();

        RenderSystem::update(
            m_ECSManager->m_Entities,
            m_ECSManager->m_Positions,
            m_ECSManager->m_Colliders,
            m_ECSManager->m_Shapes
        );

        m_Instrumentation->Draw();

        m_Renderer->Draw();
    }
}
