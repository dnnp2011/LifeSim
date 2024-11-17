#pragma once

#include <imgui_impl_glfw.h>
#include <Instrumentation.h>
#include <ServiceContainer.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"


class InputManager {
private:
    Renderer* m_renderer{ Container::Resolve<Renderer>().get() };
    Instrumentation* m_instrumentation{ Container::Resolve<Instrumentation>().get() };

    float m_maxZoom{ 10.0f };
    float m_minZoom{ 0.1f };

public:
    InputManager() {
        // FIXME: These should already be mounted
        glfwSetKeyCallback(m_renderer->m_Window, ImGui_ImplGlfw_KeyCallback);
        glfwSetScrollCallback(m_renderer->m_Window, ImGui_ImplGlfw_ScrollCallback);
    }

    [[nodiscard]]
    int Poll() const;
};
