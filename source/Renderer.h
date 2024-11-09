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
    GLFWwindow *window = nullptr;
    ImGuiIO *io        = nullptr;

private:
    bool show_demo_window    = false;
    bool show_another_window = false;
    ImVec4 clear_color{ 0.45f, 0.55f, 0.60f, 1.00f };
    float window_rounding{ 5.0f };
    float popupRounding{ 5.0f };
    float windowBgAlpha{ 0.8f };
    ImVec2 windowMinSize{ 1280, 720 };
    ImVec2 windowSize{ 1920, 1080 }; // Figure out how to reference this from within a static method

public:
    Renderer();

    ~Renderer();

    void prerender() const;

    void render() const;

    ImVec2 ScreenToViewport(const ImVec2 &screen_coords);

    void drawCircle(const ImVec2 &center, float radius, ImU32 color);

    void drawRectangle(const ImVec2 &topLeft, const ImVec2 &bottomRight, ImU32 color);

    void drawTriangle(const ImVec2 &p1, const ImVec2 &p2, const ImVec2 &p3, ImU32 color);

    void drawShape(ShapeType shape, ImVec2 position, int size, ImU32 color);

private:
    void drawGui();
};
