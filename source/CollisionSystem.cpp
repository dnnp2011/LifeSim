#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CollisionSystem.h"


void CollisionSystem::update(const std::vector<Entity> &entities, std::unordered_map<int, ImVec2> &positions, std::unordered_map<int, Collider> &colliders) {
    entitiesToStop.clear();

    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            auto &entityA = entities[i];
            auto &entityB = entities[j];

            if (positions.find(entityA.id) != positions.end() && colliders.find(entityA.id) != colliders.end() && positions.find(entityB.id) != positions.end() && colliders.find(entityB.id) != colliders.end()) {
                if (isColliding(positions[entityA.id], colliders[entityA.id], positions[entityB.id], colliders[entityB.id])) {
                    entitiesToStop.insert(entityA.id);
                    entitiesToStop.insert(entityB.id);
                }
            }
        }
    }
}

bool CollisionSystem::isColliding(const ImVec2 &posA, const Collider &colA, const ImVec2 &posB, const Collider &colB) {
    return !(posA.x + colA.width < posB.x || posA.x >
             posB.x + colB.width || posA.y + colA.height < posB.y || posA.y >
             posB.y + colB.height
    );
}
