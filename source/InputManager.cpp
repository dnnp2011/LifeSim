#include "InputManager.h"


int InputManager::Poll() const
{
    glfwPollEvents();

    // Skip rendering if window is minimized
    if (glfwGetWindowAttrib(m_renderer->m_Window, GLFW_ICONIFIED) != 0) {
        ImGui_ImplGlfw_Sleep(50);

        return -1;
    }

    //region IO Testing -----------------------------------------
    static float zoom{ 1.0f };
    const ImGuiIO* io = &ImGui::GetIO();

    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
        fprintf(stdout, "ImGui::Arrow Up pressed\n");
        zoom += 0.1f;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
        fprintf(stdout, "ImGui::Arrow Down pressed\n");
        zoom -= 0.1f;
    }

    if (std::abs(io->MouseWheel) > 0.0f || io->MouseWheelH != 0.0f) {
        zoom += io->MouseWheel;
        zoom = std::clamp(zoom, m_minZoom, m_maxZoom);

        ImGui::SetWindowFontScale(zoom);

        // auto t{ frameTime / 0.3f }; // 300ms to reach target zoom
        const auto t{ m_instrumentation->GetFrameTime() / 0.15f }; // 300ms to reach target zoom
        // m_Renderer->m_Zoom = ImMath::Lerp(m_Renderer->m_Zoom, zoom, std::clamp(t, 0.0f, 3.0f)); // Works well but a bit abrupt at start and finish
        m_renderer->m_Zoom = ImMath::EaseInOut(m_renderer->m_Zoom, zoom, std::clamp(t, 0.0f, 1.0f));
    }

#if 0
    Debounce(
        [io](const std::string& id) {
            fprintf(stdout, "MouseWheel: %.2f\n", io->MouseWheel);
            fprintf(stdout, "MouseWheelH: %.2f\n", io->MouseWheelH);
            fprintf(stdout, "MouseWheel: %.2f\n", io->MouseWheel);
        },
        1000,
        "MouseWheel"
    );
#endif
    //endregion -------------------------------------------------

    return 0;
}
