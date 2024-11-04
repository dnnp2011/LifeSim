#include <iostream>

#include "Application.h"


Application::Application() {
    std::cout << "Application()" << std::endl;
}

void Application::run() {
    while (!glfwWindowShouldClose(renderer.window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Skip rendering if window is minimized
        if (glfwGetWindowAttrib(renderer.window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));

            continue;
        }

        renderer.prerender();
        worldManager.run();
        renderer.render();

        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
