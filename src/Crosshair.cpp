#include "Crosshair.h"
#include <iostream>

const char* crosshairVertexShader = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 screenSize;

void main()
{
    vec2 pos = aPos / screenSize * 2.0;
    gl_Position = vec4(pos, 0.0, 1.0);
}
)";

const char* crosshairFragmentShader = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 0.8);
}
)";

Crosshair::Crosshair() {
    setupCrosshair();
    createShader();
}

Crosshair::~Crosshair() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Crosshair::setupCrosshair() {
    // Crosshair lines (horizontal and vertical)
    float size = 10.0f;
    float thickness = 2.0f;
    
    float vertices[] = {
        // Horizontal line
        -size, -thickness,
        -size,  thickness,
         size,  thickness,
         size,  thickness,
         size, -thickness,
        -size, -thickness,
        
        // Vertical line
        -thickness, -size,
        -thickness,  size,
         thickness,  size,
         thickness,  size,
         thickness, -size,
        -thickness, -size,
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    glBindVertexArray(0);
}

void Crosshair::createShader() {
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &crosshairVertexShader, NULL);
    glCompileShader(vertexShader);
    
    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &crosshairFragmentShader, NULL);
    glCompileShader(fragmentShader);
    
    // Shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Crosshair::draw(int screenWidth, int screenHeight) {
    glUseProgram(shaderProgram);
    
    // Set screen size uniform
    int screenSizeLoc = glGetUniformLocation(shaderProgram, "screenSize");
    glUniform2f(screenSizeLoc, (float)screenWidth, (float)screenHeight);
    
    // Disable depth test for 2D overlay
    glDisable(GL_DEPTH_TEST);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
    
    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);
}
