// Dear ImGui: standalone example application for GLFW + OpenGL2, using legacy fixed pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_glfw_opengl2/ folder**
// See imgui_impl_glfw.cpp for details.

#include <windows.h>
#include <iostream>

#include "source/Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::cout << R"(Running WinMain())" << std::endl;
    MessageBox(nullptr, "Running LifeSim from WinMain", "WinMain Demo", MB_OK);

    g_Application.Run();

    return 0;
}

int main(int, char**) {
    // if constexpr (WIN64 && DEBUG) { // Windows Only
    //     MessageBox(nullptr, "Booting LifeSim!", "LifeSim", MB_OK);
    // } else if constexpr (DEBUG) {
    //     std::cout << "Press Enter to continue..." << std::endl;
    //     std::cin.get();
    // }

    g_Application.Run();

    return 0;
}
