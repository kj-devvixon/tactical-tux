#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include <GL/glew.h>

class Crosshair {
public:
    Crosshair();
    ~Crosshair();
    
    void draw(int screenWidth, int screenHeight);
    
private:
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    
    void setupCrosshair();
    void createShader();
};

#endif
