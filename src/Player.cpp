#include "Player.h"

Player::Player(glm::vec3 startPosition)
    : camera(startPosition), moveSpeed(5.0f), mouseSensitivity(0.1f),
      firstMouse(true), lastX(400.0f), lastY(300.0f),
      velocity(0.0f), isGrounded(false), jumpForce(8.0f), physics(nullptr) {
}

void Player::processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 movement(0.0f);

    // WASD movement (only horizontal)
    glm::vec3 forward = glm::normalize(glm::vec3(camera.front.x, 0.0f, camera.front.z));
    glm::vec3 right = glm::normalize(glm::vec3(camera.right.x, 0.0f, camera.right.z));
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += right;

    // Jump
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
        velocity.y = jumpForce;
        isGrounded = false;
    }

    if (glm::length(movement) > 0.0f)
        move(glm::normalize(movement), deltaTime);
}

void Player::processMouseMovement(float xPos, float yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    camera.processMouseMovement(xOffset, yOffset);
}

void Player::processMouseScroll(float yOffset) {
    camera.processMouseScroll(yOffset);
}

void Player::update(float deltaTime) {
    applyGravity(deltaTime);
}

void Player::move(glm::vec3 direction, float deltaTime) {
    glm::vec3 oldPos = camera.position;
    glm::vec3 newPos = oldPos + direction * moveSpeed * deltaTime;
    
    if (physics) {
        glm::vec3 playerSize(Physics::PLAYER_RADIUS * 2.0f, Physics::PLAYER_HEIGHT, Physics::PLAYER_RADIUS * 2.0f);
        camera.position = physics->resolveCollision(oldPos, newPos, playerSize);
    } else {
        camera.position = newPos;
    }
}

void Player::applyGravity(float deltaTime) {
    if (physics) {
        glm::vec3 playerSize(Physics::PLAYER_RADIUS * 2.0f, Physics::PLAYER_HEIGHT, Physics::PLAYER_RADIUS * 2.0f);
        
        // Check if on ground
        isGrounded = physics->isOnGround(camera.position, playerSize);
        
        if (!isGrounded) {
            velocity.y += Physics::GRAVITY * deltaTime;
        } else {
            if (velocity.y < 0.0f) {
                velocity.y = 0.0f;
            }
        }
        
        // Apply vertical velocity
        glm::vec3 oldPos = camera.position;
        glm::vec3 newPos = oldPos + glm::vec3(0.0f, velocity.y * deltaTime, 0.0f);
        camera.position = physics->resolveCollision(oldPos, newPos, playerSize);
        
        // If we hit something, stop vertical velocity
        if (camera.position.y == oldPos.y && velocity.y != 0.0f) {
            velocity.y = 0.0f;
        }
    }
}
