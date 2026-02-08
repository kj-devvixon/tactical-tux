#include "MapEditor.h"
#include <fstream>
#include <sstream>
#include <iostream>

MapEditor::MapEditor() 
    : editorMode(false), selectedObject(-1) {
    newObjectTemplate = MapObject(glm::vec3(0.0f, 0.5f, 0.0f), 
                                   glm::vec3(1.0f), 
                                   glm::vec3(0.8f, 0.2f, 0.2f),
                                   true);
}

void MapEditor::addObject(MapObject obj) {
    objects.push_back(obj);
}

void MapEditor::removeObject(int index) {
    if (index >= 0 && index < objects.size()) {
        objects.erase(objects.begin() + index);
    }
}

void MapEditor::update() {
    // This will be called from main loop
    // ImGui rendering happens in main.cpp
}

void MapEditor::saveMap(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to save map: " << filename << std::endl;
        return;
    }
    
    file << objects.size() << "\n";
    for (const auto& obj : objects) {
        file << obj.position.x << " " << obj.position.y << " " << obj.position.z << " "
             << obj.scale.x << " " << obj.scale.y << " " << obj.scale.z << " "
             << obj.color.r << " " << obj.color.g << " " << obj.color.b << " "
             << obj.isWall << "\n";
    }
    
    file.close();
    std::cout << "Map saved: " << filename << std::endl;
}

void MapEditor::loadMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to load map: " << filename << std::endl;
        return;
    }
    
    objects.clear();
    
    int count;
    file >> count;
    
    for (int i = 0; i < count; i++) {
        MapObject obj;
        file >> obj.position.x >> obj.position.y >> obj.position.z
             >> obj.scale.x >> obj.scale.y >> obj.scale.z
             >> obj.color.r >> obj.color.g >> obj.color.b
             >> obj.isWall;
        objects.push_back(obj);
    }
    
    file.close();
    std::cout << "Map loaded: " << filename << " (" << count << " objects)" << std::endl;
}
