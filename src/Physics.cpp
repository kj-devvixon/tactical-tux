#include "Physics.h"
#include <algorithm>

const float Physics::GRAVITY = -20.0f;
const float Physics::PLAYER_HEIGHT = 1.8f;
const float Physics::PLAYER_RADIUS = 0.3f;

AABB::AABB(glm::vec3 position, glm::vec3 size) {
    min = position - size * 0.5f;
    max = position + size * 0.5f;
}

bool AABB::intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

AABB CollisionBox::getAABB() const {
    return AABB(position, size);
}

Physics::Physics() {
}

void Physics::addCollisionBox(glm::vec3 position, glm::vec3 size) {
    collisionBoxes.push_back(CollisionBox(position, size));
}

bool Physics::checkCollision(glm::vec3 position, glm::vec3 size) {
    AABB playerBox(position, size);
    
    for (const auto& box : collisionBoxes) {
        if (playerBox.intersects(box.getAABB())) {
            return true;
        }
    }
    return false;
}

glm::vec3 Physics::resolveCollision(glm::vec3 oldPos, glm::vec3 newPos, glm::vec3 size) {
    glm::vec3 result = newPos;
    
    // Try X axis
    glm::vec3 testPos = glm::vec3(newPos.x, oldPos.y, oldPos.z);
    if (checkCollision(testPos, size)) {
        result.x = oldPos.x;
    }
    
    // Try Y axis
    testPos = glm::vec3(result.x, newPos.y, oldPos.z);
    if (checkCollision(testPos, size)) {
        result.y = oldPos.y;
    }
    
    // Try Z axis
    testPos = glm::vec3(result.x, result.y, newPos.z);
    if (checkCollision(testPos, size)) {
        result.z = oldPos.z;
    }
    
    return result;
}

bool Physics::isOnGround(glm::vec3 position, glm::vec3 size) {
    glm::vec3 testPos = position - glm::vec3(0.0f, 0.05f, 0.0f);
    return checkCollision(testPos, size);
}
void Physics::clear() {
    collisionBoxes.clear();
}
