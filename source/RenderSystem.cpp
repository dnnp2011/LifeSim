#include "RenderSystem.h"

#include <unordered_map>
#include <vector>

#include "common.h"
#include "Renderer.h"

// TODO: Refactor to use multithreading to draw in parallel, if possible
void RenderSystem::update(const std::vector<Entity> &entities, const std::unordered_map<int, ImVec2> &positions, const std::unordered_map<int, ShapeType> &shapes) {
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

        if (position != positions.end() && shape != shapes.end()) {
            Renderer::drawShape(shape->second, position->second, 50, IM_COL32(255, 255, 255, 255));
        }
    }

    ImGui::End();
}
