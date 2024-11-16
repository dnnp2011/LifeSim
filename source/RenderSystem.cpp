#include <Common.h>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "RenderSystem.h"
#include "Application.h"


// TODO: Refactor to use multithreading to draw in parallel, if possible
void RenderSystem::update(
    const EntityBuffer& entities,
    const PositionBuffer& positions,
    const ColliderBuffer& colliders,
    const ShapeTypeBuffer& shapes
) {
    constexpr ImGuiWindowFlags window_flags{
        ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoBackground
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
    };

    const ImGuiViewport* viewport{ ImGui::GetMainViewport() };
    const ImVec2 viewport_pos{ viewport->Pos };
    const ImVec2 viewport_size{ viewport->Size };

    ImGui::SetNextWindowPos(viewport_pos);
    ImGui::SetNextWindowSize(viewport_size);

    ImGui::Begin("Render Window", nullptr, window_flags);
    // ImGui::ScaleWindowsInViewport(viewport, 1.0f);

    const auto zoom = g_Application.m_Renderer->m_Zoom;

    for (const auto& [id]: entities) {
        const auto position{ positions.find(id) };
        const auto shape{ shapes.find(id) };
        const auto collider{ colliders.find(id) };

        if (position != positions.end() && shape != shapes.end()) {
            switch (shape->second) {
                case ShapeType::Rectangle:
                    g_Application.m_Renderer->DrawRect(
                        ImVec2(position->second.x - ((collider->second.width * zoom) / 2), position->second.y - ((collider->second.height * zoom) / 2)),
                        ImVec2(position->second.x + ((collider->second.width * zoom) / 2), position->second.y + ((collider->second.height * zoom) / 2)),
                        IM_COL32(255, 255, 255, 255)
                    );
                    break;
                case ShapeType::Circle:
                    g_Application.m_Renderer->DrawCircle(
                        ImVec2(position->second.x, position->second.y),
                        static_cast<float>(std::max(collider->second.width * zoom, collider->second.height * zoom)) / 2,
                        IM_COL32(255, 255, 255, 255)
                    );
                    break;
                case ShapeType::Triangle:
                    g_Application.m_Renderer->DrawTriangle(
                        ImVec2(position->second.x, position->second.y - ((collider->second.height * zoom) / 2)),
                        ImVec2(position->second.x + ((collider->second.width * zoom) / 2), position->second.y + ((collider->second.height * zoom) / 2)),
                        ImVec2(position->second.x - ((collider->second.width * zoom) / 2), position->second.y + ((collider->second.height * zoom) / 2)),
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
