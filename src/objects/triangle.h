#pragma once
#include <glm/glm.hpp>

struct Vertex {

    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
};

struct Triangle {

    Vertex vertices[3];
};
