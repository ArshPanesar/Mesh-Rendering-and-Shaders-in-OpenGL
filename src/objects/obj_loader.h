#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

class ObjLoader {
private:

    std::vector<std::string> split_string(const std::string& str, const char& delimiter);

public:
    class ObjTriangleFace {
    public:
        glm::ivec3 vert_indices;
        glm::ivec3 normal_indices;
    };

public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<ObjTriangleFace> faces;
    
public:
    ObjLoader();
    ~ObjLoader();

    // Loads Vertices and Faces from a .obj and stores them in
    // the vertices and faces lists 
    bool load(const char* filename);

    // Generate Smooth Normals for each Vertex
    void generate_smooth_normals();

    // Generate Flat Normals for each Vertex
    void generate_flat_normals();
};

