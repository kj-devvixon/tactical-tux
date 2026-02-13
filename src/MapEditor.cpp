#include "MapEditor.h"
#include <fstream>
#include <iostream>
#include <sstream>

MapEditor::MapEditor() : editorMode(false), selectedObject(0) {}

void MapEditor::addObject(MapObject obj) {
    objects.push_back(obj);
}

void MapEditor::removeObject(int index) {
    if (index >= 0 && index < objects.size()) {
        objects.erase(objects.begin() + index);
    }
}

void MapEditor::update() {
    // Tutaj możesz dodać logikę aktualizacji edytora
}

void MapEditor::saveMap(const std::string& filename) {
    std::ofstream mapFile(filename);

    if (!mapFile.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku do zapisu: " << filename << std::endl;
        return;
    }

    for (const auto& obj : objects) {
        // Zapisujemy: pozycja(x,y,z) skala(x,y,z) kolor(r,g,b) czyWall
        mapFile << obj.position.x << " " << obj.position.y << " " << obj.position.z << " "
                << obj.scale.x << " " << obj.scale.y << " " << obj.scale.z << " "
                << obj.color.r << " " << obj.color.g << " " << obj.color.b << " "
                << obj.isWall << "\n";
    }

    mapFile.close();
    std::cout << "Mapa zapisana: " << filename << std::endl;
}

void MapEditor::loadMap(const std::string& filename) {
    std::ifstream mapFile(filename);

    if (!mapFile.is_open()) {
        std::cerr << "Informacja: Brak pliku mapy " << filename << ". Startuje czysta mapa." << std::endl;
        return;
    }

    objects.clear();

    std::string line;
    while (std::getline(mapFile, line)) {
        std::stringstream ss(line);
        glm::vec3 pos, scl, col;
        bool wall;

        if (ss >> pos.x >> pos.y >> pos.z >> scl.x >> scl.y >> scl.z >> col.r >> col.g >> col.b >> wall) {
            addObject(MapObject(pos, scl, col, wall));
        }
    }

    mapFile.close();
    std::cout << "Mapa wczytana: " << filename << " (Obiektow: " << objects.size() << ")" << std::endl;
}
