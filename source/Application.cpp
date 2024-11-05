#include <iostream>
// #include <thread>

#include "Application.h"


Application::Application() {
    std::cout << "Application()" << std::endl;
}

void Application::run() {
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
