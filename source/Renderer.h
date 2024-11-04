#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "common.h"


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

    static ImVec2 ScreenToViewport(const ImVec2 &screen_coords);

    static void drawShape(ShapeType shape, ImVec2 p, int size, ImU32 color);

private:
    void drawGui();
};
