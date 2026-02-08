#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct MapObject {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 color;
    bool isWall;
    
    MapObject(glm::vec3 pos = glm::vec3(0.0f), 
              glm::vec3 scl = glm::vec3(1.0f), 
              glm::vec3 col = glm::vec3(0.8f, 0.2f, 0.2f),
              bool wall = false)
        : position(pos), scale(scl), color(col), isWall(wall) {}
};

class MapEditor {
public:
    std::vector<MapObject> objects;
    bool editorMode;
    int selectedObject;
    
    MapObject newObjectTemplate;
    
    MapEditor();
    
    void addObject(MapObject obj);
    void removeObject(int index);
    void update();
    
    void saveMap(const std::string& filename);
    void loadMap(const std::string& filename);
    
    // Get all objects for rendering
    const std::vector<MapObject>& getObjects() const { return objects; }
    
    void toggleEditorMode() { editorMode = !editorMode; }
    bool isEditorMode() const { return editorMode; }
};

#endif
