#include <GLFW/glfw3.h>

#include "Application.h"


void Application::Run() {
    static constexpr auto fixedDeltaTime{ 1.0f / 30.0f }; // Fixed timestep (30 updates per second)
    static float physicsAccumulator{ 0.0f };

    m_Instrumentation->Start();

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
