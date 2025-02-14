#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <Common.h>
#include <imgui_impl_glfw.h>


class Renderer {
public:
    GLFWwindow* m_Window{ nullptr };
    ImGuiIO* m_IO{ nullptr };
    mutable float m_Zoom{ 1.0f };
    mutable unsigned int m_BufferHeight{ 720 };
    mutable unsigned int m_BufferWidth{ 1280 };
    mutable unsigned int m_WindowHeight{ 720 };
    mutable unsigned int m_WindowWidth{ 1280 };
    mutable unsigned int m_WindowXOffset{ 0 };
    mutable unsigned int m_WindowYOffset{ 0 };

private:
    bool m_showDemoWindow{ false };
    bool m_showAnotherWindow{ false };
    ImVec4 m_backgroundColor{ 0.0f, 0.0f, 0.0f, 1.00f }; // Window Background Color
    float m_windowRounding{ 5.0f };
    float m_popupRounding{ 5.0f };
    float m_windowBgAlpha{ 0.8f };
    ImVec2 m_windowMinSize{ 1280, 720 };
    ImVec2 m_windowMaxSize{ 3840, 2160 };
    ImVec2 m_windowSize{ 1280, 720 };

public:
    Renderer();

    ~Renderer();

    void NewFrame() const;

    void Draw() const;

    [[nodiscard]] ImVec2 ScreenToViewport(const ImVec2& screen_coords) const;

    void DrawCircle(const ImVec2& center, const float& radius, ImU32 color);

    void DrawRect(const ImVec2& topLeft, const ImVec2& bottomRight, ImU32 color);

    void DrawTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 color);

    void DrawShape(const ShapeType& shape, const ImVec2& topLeft, const int& size, ImU32 color);

private:
    void drawGui();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    static void windowPosCallback(GLFWwindow* window, int x, int y);

    static void windowSizeCallback(GLFWwindow* window, int width, int height);

    static void errorCallback(int error, const char* description);

    static void windowRefreshCallback(GLFWwindow* window);
};
