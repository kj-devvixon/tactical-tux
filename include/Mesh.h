#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void draw();

private:
    void setupMesh();
};

// Helper function to create a cube mesh
Mesh* createCube(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

// Helper function to create a floor mesh
Mesh* createFloor(float size, glm::vec3 color = glm::vec3(0.3f, 0.3f, 0.3f));

#endif
