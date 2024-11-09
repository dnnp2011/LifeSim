#pragma once

#include "Common.h"


class RenderSystem {
public:
    static void update(
        const EntityBuffer& entities,
        const PositionBuffer& positions,
        const ColliderBuffer& colliders,
        const ShapeTypeBuffer& shapes
    );
};
