#include <iostream>
// #include <thread>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Application.h"


Application::Application() {
    std::cout << "Starting LifeSim Application" << std::endl;
}

void Application::Run() {
    // std::thread entityThread(worldManager.run, std::ref(worldManager), 60.0);

    while (!glfwWindowShouldClose(renderer.window)) {
        glfwPollEvents();

        // Skip rendering if window is minimized
        if (glfwGetWindowAttrib(renderer.window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);

            continue;
        }

        renderer.prerender();
        worldManager.run(); // TODO: Decouple WorldManager/ECSManager from Render Pipeline
        renderer.render();

        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // entityThread.join();
}
