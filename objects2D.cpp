#include "objects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* objects2D::CreateSquare(const std::string& name, glm::vec3 lowerLeftCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = lowerLeftCorner;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2 };
    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateRectangle(const std::string& name, glm::vec3 lowerLeftCorner, float width, float height, glm::vec3 color)
{
    glm::vec3 corner = lowerLeftCorner;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2 };

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* objects2D::CreateRhombus(const std::string& name, glm::vec3 leftPosition, glm::vec3 color)
{
    glm::vec3 corner = leftPosition;
    std::vector<VertexFormat> vertices =
    {
        // romb
        VertexFormat(corner + glm::vec3(-40, 0, 0), color),
        VertexFormat(corner + glm::vec3(-15, 45, 0), color),
        VertexFormat(corner + glm::vec3(12, 0, 0), color),
        VertexFormat(corner + glm::vec3(-15, -45, 0), color),
        
        // dreptunghi
        VertexFormat(corner + glm::vec3(5, -10, 0), color),
        VertexFormat(corner + glm::vec3(40, -10, 0), color),
        VertexFormat(corner + glm::vec3(40, 10, 0), color),
        VertexFormat(corner + glm::vec3(5, 10, 0), color)
    };

    Mesh* rhombus = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
    };

    rhombus->InitFromData(vertices, indices);
    return rhombus;
}

Mesh* objects2D::CreateHexagon(const std::string& name, glm::vec3 leftPosition, glm::vec3 color, glm::vec3 color2)
{
    glm::vec3 center = leftPosition;
   
        int x1 = 16, y1 = 7;
        int x2 = 7, y2 = 16;
        int x3 = 13, y3 = 13;

    std::vector<VertexFormat> vertices =
    {
        // varfuri hexagon mare
        VertexFormat(center + glm::vec3(-25 - x1, 5 + y1, 1), color), // stanga hexagon X
        VertexFormat(center + glm::vec3(-20 - x3, -20 - y3, 1), color), // jos stanga
        VertexFormat(center + glm::vec3(7 + x2, -26 - y2, 1), color), // jos dreapta X
        VertexFormat(center + glm::vec3(26 + x1, -6 - y1, 1), color), // dreapta hexagon X
        VertexFormat(center + glm::vec3(20 + x3, 20 + y3, 1), color), // sus dreapta 
        VertexFormat(center + glm::vec3(-6 - x2, 25 + y2, 1), color), // sus stanga X
        // varfuri hexagon mic
        VertexFormat(center + glm::vec3(-27, 7, 2), color2), // stanga hexagon X
        VertexFormat(center + glm::vec3(-22, -22, 2), color2), // jos stanga
        VertexFormat(center + glm::vec3(9, -28, 2), color2), // jos dreapta X
        VertexFormat(center + glm::vec3(28, -8, 2), color2), // dreapta hexagon X
        VertexFormat(center + glm::vec3(22, 22, 2), color2), // sus dreapta 
        VertexFormat(center + glm::vec3(-8, 27, 2), color2), // sus stanga X

    };

    Mesh* hexa = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 2, 3,
        2, 3, 4,
        3, 4, 5,
        4, 5, 0,
        5, 0, 1,
        1, 3, 5,

        6, 7, 8,
        7, 8, 9,
        8, 9, 10,
        9, 10, 11,
        10, 11, 6,
        11, 6, 7,
        7, 9, 11,
    };

    hexa->InitFromData(vertices, indices);
    return hexa;
}

Mesh* objects2D::CreateStar(const std::string& name, glm::vec3 centerPosition, glm::vec3 color)
{
    glm::vec3 center = centerPosition;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-40, 10, 0), color), 
        VertexFormat(center + glm::vec3(-10, 10, 0), color),
        VertexFormat(center + glm::vec3(0, 36, 0), color),
        VertexFormat(center + glm::vec3(10, 10, 0), color),
        VertexFormat(center + glm::vec3(40, 10, 0), color),

        VertexFormat(center + glm::vec3(15, -10, 0), color),
        VertexFormat(center + glm::vec3(22, -36, 0), color),
        VertexFormat(center + glm::vec3(0, -20, 0), color),
        VertexFormat(center + glm::vec3(-22, -36, 0), color),
        VertexFormat(center + glm::vec3(-15, -10, 0), color),
        
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = {
        9, 0, 1,
        1, 2, 3,
        3, 4, 5,
        5, 6, 7,
        7, 8, 9,
        2, 5, 8,
        2, 6, 9,
        0, 3, 6,
        1, 4, 8,
    };

    star->InitFromData(vertices, indices);
    return star;
}
