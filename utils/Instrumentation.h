#pragma once

#include <Common.h>
#include <windows.h>
#include <GLFW/glfw3.h>


class Instrumentation {
private:
    LARGE_INTEGER m_frequency{};
    LARGE_INTEGER m_start{};
    LARGE_INTEGER m_end{};

    int m_windowsFrames{ 0 };
    int m_totalFrames{ 0 };
    int m_frameCount{ 0 };
    int m_physicsTickCount{ 0 };

    float m_physicsFrameTime{ 0.0f };
    float m_lastPhysicsTick{ 0.0f };
    float m_lastPhysicsTime{ 0.0f };
    float m_physicsTps{ 0.0f }; // Tick per second

    float m_currentTime{ 0.0 };
    float m_frameTime{ 0.0f };
    float m_lastFrame{ 0.0f };
    float m_lastTime{ 0.0f };
    float m_fps{ 0.0f }; // Frames per second
    float m_cpuUsage{ 0.0f };

    double m_windowsElapsedTime{ 0.0f };
    double m_windowsFrameTime{ 0.0 };
    double m_windowsFps{ 0.0 };

public:
    Instrumentation() = default;

    void Start() {
        // -- Start Windows Performance Counter -- //
        QueryPerformanceFrequency(&m_frequency);
        QueryPerformanceCounter(&m_start);
    }

    [[nodiscard]]
    int GetTotalFrames() const {
        return m_totalFrames;
    }

    [[nodiscard]]
    float GetFrameTime() const {
        return m_frameTime;
    }

    void MeasurePhysics() {
        if (m_currentTime - m_lastPhysicsTick >= 1.0f) {
            m_physicsTps       = static_cast<float>(m_physicsTickCount) / (m_currentTime - m_lastPhysicsTick);
            m_lastPhysicsTick  = m_currentTime;
            m_physicsTickCount = 0;
        }

        m_physicsFrameTime = m_currentTime - m_lastPhysicsTime;
        m_lastPhysicsTime  = m_currentTime;
    }

    void Measure() {
        m_totalFrames++;
        m_physicsTickCount++;

        calculateFps();
        calculateCpuUsage();
        calculateWindowsFps();
    }

    void Draw() const {
        static ImVec2 panelSize{ 250, 175 };
        static ImVec2 maxPanelSize{ 400, 400 };
        static ImVec2 padding{ 10, 10 };
        static bool showPerformanceMetrics{ true };

        ImGui::SetNextWindowSize(panelSize);
        ImGui::SetNextWindowSizeConstraints(panelSize, maxPanelSize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);

        if (!ImGui::Begin("Performance Metrics", &showPerformanceMetrics, ImGuiWindowFlags_AlwaysAutoResize))
            goto cleanup;

        ImGui::Text("Frame Time: %.3f ms", m_frameTime * 1000.0f);
        ImGui::Text("FPS: %.1f", m_fps);
        ImGui::Text("TPS: %.1f", m_physicsTps);
        ImGui::Text("Windows Frame Time: %.3f ms", m_windowsFrameTime * 1000.0f);
        ImGui::Text("Physics Frame Time: %.3f ms", m_physicsFrameTime * 1000.0f);
        ImGui::Text("Windows FPS: %.1f", m_windowsFps);
        ImGui::Text("CPU Usage: %.1f%%", m_cpuUsage);

    cleanup:
        ImGui::End();
        ImGui::PopStyleVar();
    }

private:
    void calculateCpuUsage() {
        static FILETIME prevSysKernel, prevSysUser;
        static FILETIME prevProcKernel, prevProcUser;
        FILETIME sysIdle, sysKernel, sysUser;
        FILETIME procCreation, procExit, procKernel, procUser;

        if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) || !GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
            m_cpuUsage = 0.0f;

        if (prevSysKernel.dwLowDateTime != 0 && prevSysKernel.dwHighDateTime != 0) {
            ULARGE_INTEGER sysKernelDiff, sysUserDiff;
            ULARGE_INTEGER procKernelDiff, procUserDiff;

            sysKernelDiff.QuadPart = (reinterpret_cast<ULARGE_INTEGER&>(sysKernel).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevSysKernel).QuadPart);
            sysUserDiff.QuadPart   = (reinterpret_cast<ULARGE_INTEGER&>(sysUser).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevSysUser).QuadPart);

            procKernelDiff.QuadPart = (reinterpret_cast<ULARGE_INTEGER&>(procKernel).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevProcKernel).QuadPart);
            procUserDiff.QuadPart   = (reinterpret_cast<ULARGE_INTEGER&>(procUser).QuadPart - reinterpret_cast<ULARGE_INTEGER&>(prevProcUser).QuadPart);

            const ULONGLONG sysTotal  = sysKernelDiff.QuadPart + sysUserDiff.QuadPart;
            const ULONGLONG procTotal = procKernelDiff.QuadPart + procUserDiff.QuadPart;

            if (sysTotal > 0) {
                m_cpuUsage = (static_cast<float>(procTotal) / sysTotal) * 100.0f;
            }
        }

        prevSysKernel  = sysKernel;
        prevSysUser    = sysUser;
        prevProcKernel = procKernel;
        prevProcUser   = procUser;

        m_cpuUsage = 0.0f;
    }

    void calculateFps() {
        m_currentTime = static_cast<float>(glfwGetTime());

        //region Calculate FPS --------------------------------------
        m_frameCount++;

        if (m_currentTime - m_lastFrame >= 1.0f) {
            m_fps        = static_cast<float>(m_frameCount) / (m_currentTime - m_lastFrame);
            m_lastFrame  = m_currentTime;
            m_frameCount = 0;
        }

        m_frameTime = m_currentTime - m_lastTime;
        m_lastTime  = m_currentTime;
        //endregion -------------------------------------------------
    }

    // TODO: Use this example to fix the Windows FPS calculation
    void calculateWindowsFps() {
        QueryPerformanceCounter(&m_end);

        m_windowsElapsedTime = static_cast<double>(m_end.QuadPart - m_start.QuadPart) / m_frequency.QuadPart;
        m_windowsFrameTime   = 1.0 / m_windowsElapsedTime;
        m_windowsFps         = 1.0 / m_windowsFrameTime;
        m_windowsFrames++;

        if (m_windowsElapsedTime >= 1.0) {
            m_windowsFrames = 0;

            QueryPerformanceCounter(&m_start);
        }
    }
};
