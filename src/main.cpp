#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "Shader.h"
#include "Mesh.h"
#include "Player.h"
#include "Physics.h"
#include "Crosshair.h"
#include "MapEditor.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Player* globalPlayer = nullptr;
MapEditor* globalEditor = nullptr;
Physics* globalPhysics = nullptr;
bool editorMode = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool raycast(glm::vec3 origin, glm::vec3 direction, const std::vector<CollisionBox>& boxes, float maxDistance, glm::vec3& hitPoint);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tactical Tux - FPS", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    Physics physics;
    globalPhysics = &physics;
    
    Player player(glm::vec3(0.0f, 3.0f, 5.0f));
    player.setPhysics(&physics);
    globalPlayer = &player;

    Crosshair crosshair;
    
    MapEditor mapEditor;
    globalEditor = &mapEditor;

    Mesh* floor = createFloor(20.0f, glm::vec3(0.2f, 0.35f, 0.25f));
    
    mapEditor.loadMap("map.txt");
    
    if (mapEditor.objects.empty()) {
        physics.addCollisionBox(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(40.0f, 1.0f, 40.0f));
        
        mapEditor.addObject(MapObject(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f), glm::vec3(0.8f, 0.2f, 0.2f), true));
        mapEditor.addObject(MapObject(glm::vec3(3.0f, 0.5f, 2.0f), glm::vec3(1.0f), glm::vec3(0.2f, 0.8f, 0.2f), true));
        mapEditor.addObject(MapObject(glm::vec3(-3.0f, 0.5f, -2.0f), glm::vec3(1.0f), glm::vec3(0.2f, 0.2f, 0.8f), true));
        mapEditor.addObject(MapObject(glm::vec3(8.0f, 1.5f, 0.0f), glm::vec3(1.0f, 3.0f, 10.0f), glm::vec3(0.5f, 0.5f, 0.5f), true));
        mapEditor.addObject(MapObject(glm::vec3(-8.0f, 1.5f, 0.0f), glm::vec3(1.0f, 3.0f, 10.0f), glm::vec3(0.5f, 0.5f, 0.5f), true));
        mapEditor.addObject(MapObject(glm::vec3(0.0f, 1.5f, 8.0f), glm::vec3(10.0f, 3.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), true));
        mapEditor.addObject(MapObject(glm::vec3(0.0f, 1.5f, -8.0f), glm::vec3(10.0f, 3.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), true));
    }
    
    for (const auto& obj : mapEditor.objects) {
        if (obj.isWall) {
            physics.addCollisionBox(obj.position, obj.scale);
        }
    }

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    std::cout << "=== TACTICAL TUX - FPS ENGINE ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Look" << std::endl;
    std::cout << "  Space - Jump" << std::endl;
    std::cout << "  Left Click - Shoot" << std::endl;
    std::cout << "  E - Editor Mode" << std::endl;
    std::cout << "  ESC - Exit\n" << std::endl;
    std::cout << "Editor (Press E):" << std::endl;
    std::cout << "  P - Place object" << std::endl;
    std::cout << "  L - Delete last" << std::endl;
    std::cout << "  S - Save map" << std::endl;
    std::cout << "  O - Load map\n" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        player.processInput(window, deltaTime);
        player.update(deltaTime);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !editorMode) {
            static float lastShot = 0.0f;
            if (currentFrame - lastShot > 0.3f) {
                glm::vec3 hitPoint;
                if (raycast(player.camera.position, player.camera.front, physics.collisionBoxes, 100.0f, hitPoint)) {
                    std::cout << "HIT! " << hitPoint.x << "," << hitPoint.y << "," << hitPoint.z << std::endl;
                }
                lastShot = currentFrame;
            }
        }

        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 projection = player.camera.getProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
        shader.setMat4("projection", projection);
        glm::mat4 view = player.camera.getViewMatrix();
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        floor->draw();

        for (const auto& obj : mapEditor.objects) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);
            model = glm::scale(model, obj.scale);
            shader.setMat4("model", model);
            Mesh* cube = createCube(obj.color);
            cube->draw();
            delete cube;
        }

        crosshair.draw(SCR_WIDTH, SCR_HEIGHT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete floor;
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (globalPlayer && !editorMode) {
        globalPlayer->processMouseMovement((float)xPos, (float)yPos);
    }
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    if (globalPlayer) {
        globalPlayer->processMouseScroll((float)yOffset);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_E) {
            editorMode = !editorMode;
            if (editorMode) {
                std::cout << "\n[EDITOR ON]" << std::endl;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                std::cout << "\n[EDITOR OFF]" << std::endl;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                globalPlayer->firstMouse = true;
            }
        }
        
        if (editorMode && globalEditor) {
            if (key == GLFW_KEY_P) {
                glm::vec3 spawnPos = globalPlayer->camera.position + globalPlayer->camera.front * 3.0f;
                MapObject newObj(spawnPos, glm::vec3(1.0f), glm::vec3(0.8f, 0.2f, 0.2f), true);
                globalEditor->addObject(newObj);
                globalPhysics->addCollisionBox(newObj.position, newObj.scale);
                std::cout << "Placed at " << spawnPos.x << "," << spawnPos.y << "," << spawnPos.z << std::endl;
            }
            
            if (key == GLFW_KEY_L && !globalEditor->objects.empty()) {
                globalEditor->removeObject(globalEditor->objects.size() - 1);
                globalPhysics->clear();
                globalPhysics->addCollisionBox(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(40.0f, 1.0f, 40.0f));
                for (const auto& obj : globalEditor->objects) {
                    if (obj.isWall) globalPhysics->addCollisionBox(obj.position, obj.scale);
                }
                std::cout << "Deleted" << std::endl;
            }
            
            if (key == GLFW_KEY_S) {
                globalEditor->saveMap("map.txt");
            }
            
            if (key == GLFW_KEY_O) {
                globalEditor->loadMap("map.txt");
                globalPhysics->clear();
                globalPhysics->addCollisionBox(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(40.0f, 1.0f, 40.0f));
                for (const auto& obj : globalEditor->objects) {
                    if (obj.isWall) globalPhysics->addCollisionBox(obj.position, obj.scale);
                }
            }
        }
    }
}

bool raycast(glm::vec3 origin, glm::vec3 direction, const std::vector<CollisionBox>& boxes, float maxDistance, glm::vec3& hitPoint) {
    float closestDist = maxDistance;
    bool hit = false;
    
    for (const auto& box : boxes) {
        AABB aabb = box.getAABB();
        glm::vec3 invDir = 1.0f / direction;
        glm::vec3 t0 = (aabb.min - origin) * invDir;
        glm::vec3 t1 = (aabb.max - origin) * invDir;
        glm::vec3 tmin = glm::min(t0, t1);
        glm::vec3 tmax = glm::max(t0, t1);
        float tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
        float tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);
        
        if (tNear < tFar && tNear > 0.0f && tNear < closestDist) {
            closestDist = tNear;
            hitPoint = origin + direction * tNear;
            hit = true;
        }
    }
    
    return hit;
}
