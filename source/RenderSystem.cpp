#include <ServiceContainer.h>

#include "Renderer.h"

#include "RenderSystem.h"


RenderSystem::RenderSystem()
{
    fprintf(stdout, "RenderSystem Instantiated\n");
}

// TODO: Refactor to use multithreading to draw in parallel, if possible
void RenderSystem::Update(const EntityData& renderBuffer)
{
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

    const auto& entities{ renderBuffer.entities };
    const auto& positions{ renderBuffer.positions };
    const auto& colliders{ renderBuffer.colliders };
    const auto& shapes{ renderBuffer.shapes };

    ImGui::SetNextWindowPos(viewport_pos);
    ImGui::SetNextWindowSize(viewport_size);

    ImGui::Begin("Render Window", nullptr, window_flags);
    // ImGui::ScaleWindowsInViewport(viewport, 1.0f);

    {
        const auto renderer = Container::Resolve<Renderer>();

        for (const auto& [id]: entities) {
            IM_ASSERT(id < positions.size() && id < colliders.size());

            const auto& position{ positions[id] };
            const auto& collider{ colliders[id] };
            const auto& shape{ shapes[id] };

            const auto zoom = renderer->m_Zoom;

            switch (shape) {
                case ShapeType::Rectangle:
                    renderer->DrawRect(
                        ImVec2(position.x - ((collider.width * zoom) / 2), position.y - ((collider.height * zoom) / 2)),
                        ImVec2(position.x + ((collider.width * zoom) / 2), position.y + ((collider.height * zoom) / 2)),
                        IM_COL32(255, 255, 255, 255)
                    );
                    break;
                case ShapeType::Circle:
                    renderer->DrawCircle(
                        ImVec2(position.x, position.y),
                        static_cast<float>(std::max(collider.width * zoom, collider.height * zoom)) / 2,
                        IM_COL32(255, 255, 255, 255)
                    );
                    break;
                case ShapeType::Triangle:
                    renderer->DrawTriangle(
                        ImVec2(position.x, position.y - ((collider.height * zoom) / 2)),
                        ImVec2(position.x + ((collider.width * zoom) / 2), position.y + ((collider.height * zoom) / 2)),
                        ImVec2(position.x - ((collider.width * zoom) / 2), position.y + ((collider.height * zoom) / 2)),
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
