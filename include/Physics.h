#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <vector>

// AABB (Axis-Aligned Bounding Box) for collision
struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB(glm::vec3 position, glm::vec3 size);
    bool intersects(const AABB& other) const;
};

// Collision object in the world
struct CollisionBox {
    glm::vec3 position;
    glm::vec3 size;
    
    CollisionBox(glm::vec3 pos, glm::vec3 sz) : position(pos), size(sz) {}
    AABB getAABB() const;
};

class Physics {
public:
    static const float GRAVITY;
    static const float PLAYER_HEIGHT;
    static const float PLAYER_RADIUS;
    
    std::vector<CollisionBox> collisionBoxes;
    
    Physics();
    
    void addCollisionBox(glm::vec3 position, glm::vec3 size);
    bool checkCollision(glm::vec3 position, glm::vec3 size);
    
    // Resolve collision and return valid position
    glm::vec3 resolveCollision(glm::vec3 oldPos, glm::vec3 newPos, glm::vec3 size);
    
    // Check if player is on ground
    bool isOnGround(glm::vec3 position, glm::vec3 size);
    
    void clear();
};

#endif
