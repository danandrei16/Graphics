#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace objects2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 lowerLeftCorner, float length, glm::vec3 color, bool fill);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 lowerLeftCorner, float width, float height, glm::vec3 color);
    Mesh* CreateRhombus(const std::string& name, glm::vec3 leftPosition, glm::vec3 color);
    Mesh* CreateHexagon(const std::string& name, glm::vec3 leftPosition, glm::vec3 color, glm::vec3 color2);
    Mesh* CreateStar(const std::string& name, glm::vec3 centerPosition, glm::vec3 color);
}
