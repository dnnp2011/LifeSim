#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <imgui_impl_glfw.h>

#include "Common.h"


class Renderer {
public:
    GLFWwindow* window = nullptr;
    ImGuiIO* io        = nullptr;

private:
    bool show_demo_window    = false;
    bool show_another_window = false;
    ImVec4 clear_color{ 0.0f, 0.0f, 0.0f, 1.00f }; // Window Background Color
    float window_rounding{ 5.0f };
    float popupRounding{ 5.0f };
    float windowBgAlpha{ 0.8f };
    ImVec2 windowMinSize{ 1280, 720 };
    ImVec2 windowSize{ 3840, 2160 };

public:
    Renderer();

    ~Renderer();

    void prepareNewFrame() const;

    void renderFrame() const;

    ImVec2 ScreenToViewport(const ImVec2& screen_coords);

    void drawCircle(const ImVec2& center, const float& radius, ImU32 color);

    void drawRectangle(const ImVec2& topLeft, const ImVec2& bottomRight, ImU32 color);

    void drawTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 color);

    void drawShape(const ShapeType& shape, const ImVec2& position, const int& size, ImU32 color);

private:
    void drawGui();
};
