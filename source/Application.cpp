#include <chrono>
#include <imgui_impl_glfw.h>
#include <iostream>
#include <windows.h>
#include <GLFW/glfw3.h>

#include "Application.h"


using Clock     = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration  = std::chrono::duration<float>;

static LARGE_INTEGER frequency;
static LARGE_INTEGER start;
static LARGE_INTEGER end;

struct WindowsMetrics {
    int windowsFrames       = 0;
    double windowsFrameTime = 0.0;
    double windowsFps       = 0.0;
};

static float GetCpuUsage() {
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
    static int windowsFrames = 0;
    static double windowsFps = 0.0;

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

Application::Application() {
    std::cout << "Starting LifeSim Application" << std::endl;
}

void Application::Run() {
    static constexpr auto fixedDeltaTime = 1.0f / 30.0f; // Fixed timestep (30 updates per second)

    static int frameCount  = 0;
    static float frameTime = 0.0f;
    static float lastFrame = 0.0f;
    static float lastTime  = 0.0f;
    static float fps       = 0.0f;

    static float physicsAccumulator = 0.0f;
    static int physicsTickCount     = 0;
    static float physicsFrameTime   = 0.0f;
    static float lastPhysicsTick    = 0.0f;
    static float lastPhysicsTime    = 0.0f;
    static float physicsTps         = 0.0f; // Tick per second

    static float cpuUsage = 0.0f;

    // -- Start Windows Performance Counter -- //
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    while (!glfwWindowShouldClose(m_Renderer.window)) {
        //region GLFW Initialization --------------------------------
        glfwPollEvents();

        // Skip rendering if window is minimized
        if (glfwGetWindowAttrib(m_Renderer.window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);

            continue;
        }
        //endregion -------------------------------------------------

        const auto currentTime = static_cast<float>(glfwGetTime());

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

        cpuUsage = GetCpuUsage();

        auto [
            windowsFrames,
            windowsFrameTime,
            windowsFps
        ] = CalculateWindowsFPS();

        m_Renderer.prepareNewFrame();

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

        m_Renderer.renderFrame();
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
    ImGui::Begin("Performance Metrics");

    ImGui::Text("Frame Time: %.3f ms", frameTime * 1000.0f);
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Windows Frame Time: %.3f ms", windowsFrameTime * 1000.0f);
    ImGui::Text("Windows FPS: %.1f", windowsFps);
    ImGui::Text("Physics Frame Time: %.3f ms", physicsFrameTime * 1000.0f);
    ImGui::Text("Physics Steps Per Second: %.1f", physicsStepsPerSecond);
    ImGui::Text("CPU Usage: %.1f%%", cpuUsage);

    ImGui::End();
}
