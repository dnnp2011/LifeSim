#include <Instrumentation.h>
#include <ServiceContainer.h>

#include <GLFW/glfw3.h>

#include "ECSManager.h"
#include "InputManager.h"
#include "RenderSystem.h"

#include "Application.h"


Application::Application():
    m_Instrumentation{ Container::Bind<Instrumentation>().get() },
    m_Renderer{ Container::Bind<Renderer>().get() },
    m_ECSManager{ Container::Bind<ECSManager>().get() },
    m_InputManager{ Container::Bind<InputManager>().get() }
{
    fprintf(stdout, "Bootstrapped Application\n");
}

void Application::Run() const
{
    fprintf(stdout, "Starting Main Loop\n");

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
            m_ECSManager->Update(fixedDeltaTime);

            physicsAccumulator -= fixedDeltaTime;

            m_Instrumentation->MeasurePhysics();
        }
        /* endregion */

        m_Renderer->NewFrame();

        RenderSystem::Update(m_ECSManager->m_RenderBuffer);

        m_Instrumentation->Draw();
        m_Renderer->Draw();
    }
}
