#include "extern/imgui/imgui.h"
#include "extern/imgui/imgui_impl_metal.h"
#include "extern/imgui/imgui_impl_osx.h"
#include <iostream>

using namespace ImGui;

int main() {
    // Create ImGui context
    ImGui::CreateContext();

    if (!ImGui_ImplOSX_Init()) {
        std::cerr << "Failed to initialize ImGui for OSX" << std::endl;
        return -1;
    }

    ImGui_ImplMetal_Init();

    // Load fonts
    ImGui::StyleColorsDark();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    // Main loop
    while (true) {
        // Poll events
        ImGui_ImplOSX_Update();
        ImGui_ImplMetal_NewFrame();
        ImGui::NewFrame();

        // Show demo window
        ImGui::ShowDemoWindow();

        // Render
        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData());

        // Cleanup
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplOSX_Shutdown();
        ImGui::DestroyContext();

        return 0;
    }
}
