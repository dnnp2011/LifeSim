#pragma once


struct EntityData;

class RenderSystem {
public:
    explicit RenderSystem();

    static void Update(const EntityData& renderBuffer);
};
