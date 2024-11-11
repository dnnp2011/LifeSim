#include <iostream>
#include <unordered_map>
#include <vector>

#include "RenderSystem.h"
#include "Application.h"
#include "Common.h"


// TODO: Refactor to use multithreading to draw in parallel, if possible
void RenderSystem::update(
    const EntityBuffer& entities,
    const PositionBuffer& positions,
    const ColliderBuffer& colliders,
    const ShapeTypeBuffer& shapes
) {
    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoBackground
            | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoFocusOnAppearing
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 viewport_pos     = viewport->Pos;
    const ImVec2 viewport_size    = viewport->Size;

    ImGui::SetNextWindowPos(viewport_pos);
    ImGui::SetNextWindowSize(viewport_size);

    ImGui::Begin("Fullscreen Window", nullptr, window_flags);

    for (const auto& [id]: entities) {
        const auto position = positions.find(id);
        const auto shape    = shapes.find(id);
        const auto collider = colliders.find(id);

        if (position != positions.end() && shape != shapes.end()) {
            switch (shape->second) {
                case ShapeType::Rectangle:
                    g_Application.m_Renderer.drawRectangle(
                        ImVec2(position->second.x - (collider->second.width / 2), position->second.y - (collider->second.height / 2)),
                        ImVec2(position->second.x + (collider->second.width / 2), position->second.y + (collider->second.height / 2)),
                        IM_COL32(255, 255, 255, 255)
                    );
                    break;
                case ShapeType::Circle:
                    g_Application.m_Renderer.drawCircle(
                        ImVec2(position->second.x, position->second.y),
                        static_cast<float>(std::max(collider->second.width, collider->second.height)) / 2,
                        IM_COL32(255, 255, 255, 255)
                    );
                    break;
                case ShapeType::Triangle:
                    g_Application.m_Renderer.drawTriangle(
                        ImVec2(position->second.x, position->second.y - (collider->second.height / 2)),
                        ImVec2(position->second.x + (collider->second.width / 2), position->second.y + (collider->second.height / 2)),
                        ImVec2(position->second.x - (collider->second.width / 2), position->second.y + (collider->second.height / 2)),
                        IM_COL32(255, 255, 255, 255)
                    );
                    break;
                default:
                    break;
            }
        }
    }

    ImGui::End();
}

void DrawDebugGui(const float cpuUsage, const float gpuUsage, const float memoryUsage, const float frameTime, const int fps) {
    ImGui::Begin("Performance Metrics", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("CPU Usage: %.2f%%", cpuUsage);
    ImGui::Text("GPU Usage: %.2f%%", gpuUsage);
    ImGui::Text("Memory Usage: %.2f%%", memoryUsage);
    ImGui::Text("Frame Time: %.2f ms", frameTime);
    ImGui::Text("FPS: %d", fps);

    ImGui::End();
}
