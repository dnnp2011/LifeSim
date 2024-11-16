#include <chrono>
#include <imgui_impl_glfw.h>
#include <iostream>
#include <windows.h>
#include <GLFW/glfw3.h>

#include "Application.h"


static LARGE_INTEGER frequency;
static LARGE_INTEGER start;
static LARGE_INTEGER end;

struct WindowsMetrics {
    int windowsFrames{ 0 };
    double windowsFrameTime{ 0.0 };
    double windowsFps{ 0.0 };
};

static float get_cpu_usage() {
    static FILETIME prevSysKernel, prevSysUser;
    static FILETIME prevProcKernel, prevProcUser;
    FILETIME sysIdle, sysKernel, sysUser;
    FILETIME procCreation, procExit, procKernel, procUser;

    if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
        !GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser)) {
        return 0.0f;
    }

    if (prevSysKernel.dwLowDateTime != 0 && prevSysKernel.dwHighDateTime != 0) {
        ULARGE_INTEGER sysKernelDiff, sysUserDiff;
        ULARGE_INTEGER procKernelDiff, procUserDiff;

        sysKernelDiff.QuadPart = (reinterpret_cast<ULARGE_INTEGER&>(sysKernel).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevSysKernel).QuadPart);
        sysUserDiff.QuadPart   = (reinterpret_cast<ULARGE_INTEGER&>(sysUser).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevSysUser).QuadPart);

        procKernelDiff.QuadPart = (reinterpret_cast<ULARGE_INTEGER&>(procKernel).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevProcKernel).QuadPart);
        procUserDiff.QuadPart   = (reinterpret_cast<ULARGE_INTEGER&>(procUser).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevProcUser).QuadPart);

        ULONGLONG sysTotal  = sysKernelDiff.QuadPart + sysUserDiff.QuadPart;
        ULONGLONG procTotal = procKernelDiff.QuadPart + procUserDiff.QuadPart;

        if (sysTotal > 0) {
            return (static_cast<float>(procTotal) / sysTotal) * 100.0f;
        }
    }

    prevSysKernel  = sysKernel;
    prevSysUser    = sysUser;
    prevProcKernel = procKernel;
    prevProcUser   = procUser;

    return 0.0f;
}

// TODO: Use this example to fix the Windows FPS calculation
static WindowsMetrics CalculateWindowsFPS() {
    static double windowsElapsedTime;
    static double windowsFrameTime;
    static int windowsFrames{ 0 };
    static double windowsFps{ 0.0 };

    QueryPerformanceCounter(&end);
    windowsElapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    windowsFrameTime   = 1.0 / windowsElapsedTime;
    windowsFps         = 1.0 / windowsFrameTime;
    windowsFrames++;

    if (windowsElapsedTime >= 1.0) {
        windowsFrames = 0;
        QueryPerformanceCounter(&start);
    }

    return { windowsFrames, windowsFrameTime, windowsFps };
}

void Application::Run() {
    static constexpr auto fixedDeltaTime{ 1.0f / 30.0f }; // Fixed timestep (30 updates per second)

    static int frameCount{ 0 };
    static float frameTime{ 0.0f };
    static float lastFrame{ 0.0f };
    static float lastTime{ 0.0f };
    static float fps{ 0.0f };

    static float physicsAccumulator{ 0.0f };
    static int physicsTickCount{ 0 };
    static float physicsFrameTime{ 0.0f };

    static float lastPhysicsTick{ 0.0f };
    static float lastPhysicsTime{ 0.0f };
    static float physicsTps{ 0.0f }; // Tick per second

    static float cpuUsage{ 0.0f };

    // -- Start Windows Performance Counter -- //
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    // NOTE: When the GLFW backend is initialized, these callbacks should already be installed, but they don't seem to be working unless I explicitly set them (and scroll events still aren't working)
    glfwSetKeyCallback(m_Renderer->m_Window, ImGui_ImplGlfw_KeyCallback);
    glfwSetScrollCallback(m_Renderer->m_Window, ImGui_ImplGlfw_ScrollCallback);

    while (!glfwWindowShouldClose(m_Renderer->m_Window)) {
        ImGui::SetNextFrameWantCaptureKeyboard(true);
        ImGui::SetNextFrameWantCaptureMouse(true);

        //region GLFW Initialization --------------------------------
        glfwPollEvents();

        // Skip rendering if window is minimized
        if (glfwGetWindowAttrib(m_Renderer->m_Window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);

            continue;
        }
        //endregion -------------------------------------------------

        const auto currentTime{ static_cast<float>(glfwGetTime()) };

        //region Calculate FPS --------------------------------------
        frameCount++;

        if (currentTime - lastFrame >= 1.0f) {
            fps        = static_cast<float>(frameCount) / (currentTime - lastFrame);
            lastFrame  = currentTime;
            frameCount = 0;
        }

        frameTime = currentTime - lastTime;
        lastTime  = currentTime;
        //endregion -------------------------------------------------

        //region Calculate Physics TPS ------------------------------
        physicsAccumulator += frameTime;
        physicsTickCount++;

        // -- Fixed Timestep Loop -- //
        while (physicsAccumulator >= fixedDeltaTime) {
            m_ECSManger.update(fixedDeltaTime);

            physicsAccumulator -= fixedDeltaTime;

            if (currentTime - lastPhysicsTick >= 1.0f) {
                physicsTps       = static_cast<float>(physicsTickCount) / (currentTime - lastPhysicsTick);
                lastPhysicsTick  = currentTime;
                physicsTickCount = 0;
            }

            physicsFrameTime = currentTime - lastPhysicsTime;
            lastPhysicsTime  = currentTime;
        }
        //endregion -------------------------------------------------

        //region IO Testing -----------------------------------------
        static float zoom{ 1.0f };
        const ImGuiIO* io = &ImGui::GetIO();

        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            std::cout << "ImGui::Arrow Up pressed" << std::endl;
            zoom += 0.1f;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            std::cout << "ImGui::Arrow Down pressed" << std::endl;
            zoom -= 0.1f;
        }

        if (std::abs(io->MouseWheel) > 0.0f || io->MouseWheelH != 0.0f) {
            zoom += io->MouseWheel;
            zoom = std::clamp(zoom, m_minZoom, m_maxZoom);

            ImGui::SetWindowFontScale(zoom);

            // auto t{ frameTime / 0.3f }; // 300ms to reach target zoom
            auto t{ frameTime / 0.15f }; // 300ms to reach target zoom
            // m_Renderer->m_Zoom = ImMath::Lerp(m_Renderer->m_Zoom, zoom, std::clamp(t, 0.0f, 3.0f)); // Works well but a bit abrupt at start and finish
            m_Renderer->m_Zoom = ImMath::EaseInOut(m_Renderer->m_Zoom, zoom, std::clamp(t, 0.0f, 1.0f));
        }

        Debounce(
            [io](const std::string& id) {
                std::cout << "--------" << std::endl;
                std::cout << id << ": " << io->MouseWheel << std::endl;
                std::cout << id << "H: " << io->MouseWheelH << std::endl;
            },
            1000,
            "MouseWheel"
        );
        //endregion -------------------------------------------------

        cpuUsage = get_cpu_usage();

        auto [
            windowsFrames,
            windowsFrameTime,
            windowsFps
        ] = CalculateWindowsFPS();

        m_Renderer->NewFrame();

        RenderSystem::update(
            m_ECSManger.m_Entities,
            m_ECSManger.m_Positions,
            m_ECSManger.m_Colliders,
            m_ECSManger.m_Shapes
        );

        DrawPerformanceGui(
            frameTime,
            fps,
            windowsFrameTime,
            windowsFps,
            physicsFrameTime,
            physicsTps,
            cpuUsage
        );

        m_Renderer->Draw();
    }
}

void Application::DrawPerformanceGui(
    const float frameTime,
    const float fps,
    const double windowsFrameTime,
    const double windowsFps,
    const float physicsFrameTime,
    const float physicsStepsPerSecond,
    const float cpuUsage
) {
    static ImVec2 panelSize{ 250, 175 };
    static ImVec2 maxPanelSize{ 400, 400 };
    static ImVec2 padding{ 10, 10 };
    static bool showPerformanceMetrics{ true };

    ImGui::SetNextWindowSize(panelSize);
    ImGui::SetNextWindowSizeConstraints(panelSize, maxPanelSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);

    if (!ImGui::Begin("Performance Metrics", &showPerformanceMetrics, ImGuiWindowFlags_AlwaysAutoResize))
        goto cleanup;

    ImGui::Text("Frame Time: %.3f ms", frameTime * 1000.0f);
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Windows Frame Time: %.3f ms", windowsFrameTime * 1000.0f);
    ImGui::Text("Windows FPS: %.1f", windowsFps);
    ImGui::Text("Physics Frame Time: %.3f ms", physicsFrameTime * 1000.0f);
    ImGui::Text("Physics Steps Per Second: %.1f", physicsStepsPerSecond);
    ImGui::Text("CPU Usage: %.1f%%", cpuUsage);

cleanup:
    ImGui::End();
    ImGui::PopStyleVar();
}
