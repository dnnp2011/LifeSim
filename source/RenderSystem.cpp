#include <unordered_map>
#include <vector>

#include "Application.h"
#include "RenderSystem.h"
#include "Common.h"


// TODO: Refactor to use multithreading to draw in parallel, if possible
void RenderSystem::update(const std::vector<Entity> &entities, const std::unordered_map<int, Position> &positions, const std::unordered_map<int, Collider> &colliders, const std::unordered_map<int, ShapeType> &shapes) {
    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
    const ImGuiViewport *viewport           = ImGui::GetMainViewport();
    const ImVec2 viewport_pos               = viewport->Pos;
    const ImVec2 viewport_size              = viewport->Size;

    ImGui::SetNextWindowPos(viewport_pos);
    ImGui::SetNextWindowSize(viewport_size);

    ImGui::Begin("Fullscreen Window", nullptr, window_flags);

    for (const auto &entity: entities) {
        auto position = positions.find(entity.id);
        auto shape    = shapes.find(entity.id);
        auto collider = colliders.find(entity.id);

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
                        std::max(collider->second.width, collider->second.height) / 2,
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
