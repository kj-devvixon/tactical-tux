#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "Physics.h"
#include <GLFW/glfw3.h>

class Player {
public:
    Camera camera;
    float moveSpeed;
    float mouseSensitivity;
    
    bool firstMouse;
    float lastX;
    float lastY;
    
    // Physics
    glm::vec3 velocity;
    bool isGrounded;
    float jumpForce;
    
    Physics* physics;

    Player(glm::vec3 startPosition = glm::vec3(0.0f, 1.0f, 3.0f));

    void processInput(GLFWwindow* window, float deltaTime);
    void processMouseMovement(float xPos, float yPos);
    void processMouseScroll(float yOffset);
    
    void update(float deltaTime);
    void setPhysics(Physics* phys) { physics = phys; }

private:
    void move(glm::vec3 direction, float deltaTime);
    void applyGravity(float deltaTime);
};

#endif
