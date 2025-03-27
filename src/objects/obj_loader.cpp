#include "obj_loader.h"

ObjLoader::ObjLoader() :
    vertices(),
    normals(),
    faces()    {

}

ObjLoader::~ObjLoader() {

    vertices.clear();
    normals.clear();
    faces.clear();
}

std::vector<std::string> ObjLoader::split_string(const std::string& str, const char& delimiter) {

    std::vector<std::string> vec;

    std::string word;
    std::stringstream ss(str);
    while(std::getline(ss, word, delimiter)) {

        vec.push_back(word);        
    }

    return vec;
}

bool ObjLoader::load(const char* filename) {

    // Open File
    std::ifstream file_reader;
    file_reader.open(filename);
    if (!file_reader.is_open()) {
        
        std::cout << "ObjLoader Failed: Cannot Open File at " << filename << "\n";
        return false;
    }

    // Read Line by Line
    std::string line;
    std::vector<std::string> words;
    while (std::getline(file_reader, line)) {

        if (line.size() <= 0)
            continue;
        
        words = split_string(line, ' ');
        
        if (words[0].compare("v") == 0) {

            // Vertices
            //
            // Ignoring First Word, Extract Each Word as a Float
            // This is the (x, y, z) components of the Vertex
            glm::vec3 new_vert;
            new_vert.x = std::stof(words[1]);
            new_vert.y = std::stof(words[2]);
            new_vert.z = std::stof(words[3]);
            
            // Add New Vertex to the List
            vertices.push_back(new_vert);
        } else if (words[0].compare("f") == 0) {

            // Faces
            //

            int num_vertices = words.size() - 1;
            bool normals_found = false;
            // If 3 Vertices, its Simply a Triangle
            if (num_vertices == 3) {

                int face_data[3];
                int normal_data[3];
                for (int i = 1; i < words.size(); ++i) {
                
                    std::vector<std::string> face_words = split_string(words[i], '/');
                    face_data[i - 1] = std::stoi(face_words[0]) - 1;  
                    if (face_words.size() > 1) {
                        
                        normal_data[i - 1] = std::stoi(face_words[2]) - 1; 
                        normals_found = true;
                    }
                }   

                ObjTriangleFace face;
                face.vert_indices = glm::ivec3(face_data[0], face_data[1], face_data[2]);
                if (normals_found)
                    face.normal_indices = glm::ivec3(normal_data[0], normal_data[1], normal_data[2]);
                
                faces.push_back(face);
                continue;
            }
            
            // If Face is a Polygon, Find the Center of the Polygon
            int* face_data = new int[num_vertices];
            int* normal_data = new int[num_vertices];
            glm::vec3 center_vertex(0.0f, 0.0f, 0.0f);
            glm::vec3 center_normal(0.0f, 0.0f, 0.0f);
            int center_vert_index = -1;
            int center_normal_index = -1;
            for (int i = 1; i < words.size(); ++i) {

                std::vector<std::string> face_words = split_string(words[i], '/');
                int vert_index = std::stoi(face_words[0]) - 1;
                
                int normal_index = -1;
                if (face_words.size() > 1) {
                    
                    normal_index = std::stoi(face_words[2]) - 1;
                    normals_found = true;
                }

                center_vertex += vertices[vert_index];
                face_data[i - 1] = vert_index;
                
                if (normal_index > -1) {
                    
                    center_normal += normals[normal_index];
                    normal_data[i - 1] = normal_index;
                }
            }
            center_vertex /= num_vertices;
            center_normal /= num_vertices;

            // Add this Vertex into the List of Vertices
            vertices.push_back(center_vertex);
            center_vert_index = vertices.size() - 1;
            if (normals_found) {
                
                normals.push_back(center_normal);
                center_normal_index = normals.size() - 1;
            }

            // Now, make a Triangle Fan with the center_vertex as the Center
            for (int i = 0; i < num_vertices; ++i) {

                int j = (i + 1) % num_vertices;

                ObjTriangleFace face;
                face.vert_indices = glm::ivec3(center_vert_index, face_data[i], face_data[j]);
                if (normals_found)
                    face.normal_indices = glm::ivec3(center_normal_index, normal_data[i], normal_data[j]);

                faces.push_back(face);
            }

            delete face_data;
            face_data = nullptr;
        } else if (words[0].compare("vn") == 0) {
            
            // Vertex Normals
            glm::vec3 normal;
            normal.x = std::stof(words[1]);
            normal.y = std::stof(words[2]);
            normal.z = std::stof(words[3]);
            
            // Add New Vertex to the List
            normals.push_back(normal);
        }
    }

    return true;
}

void ObjLoader::generate_smooth_normals() {

    normals.clear();
    normals.resize(vertices.size());
    for (int i = 0; i < normals.size(); ++i)
        normals[i] = glm::vec3(0.0f);

    for (int i = 0; i < faces.size(); ++i) {

        ObjTriangleFace& face = faces[i];

        // Construct Normal
        glm::vec3 verts[] = { vertices[face.vert_indices.x], vertices[face.vert_indices.y], vertices[face.vert_indices.z] };
        glm::vec3 v0_v1 = verts[0] - verts[1];
        glm::vec3 v1_v2 = verts[1] - verts[2];
        glm::vec3 normal = glm::cross(v0_v1, v1_v2);
        
        // Add Normal to Face
        normals[ face.vert_indices.x ] += normal;
        normals[ face.vert_indices.y ] += normal;
        normals[ face.vert_indices.z ] += normal;
        
        face.normal_indices = glm::ivec3(face.vert_indices.x, face.vert_indices.y, face.vert_indices.z);
    }

    // Normalize
    for (int i = 0; i < normals.size(); ++i)
        normals[i] = glm::normalize(normals[i]);
}

void ObjLoader::generate_flat_normals() {

    normals.clear();
    normals.resize(vertices.size());
    for (int i = 0; i < normals.size(); ++i)
        normals[i] = glm::vec3(0.0f);

    bool* calculated = new bool[normals.size()];
    std::vector<glm::vec3>* neighbour_edges = new std::vector<glm::vec3>[normals.size()];
    for (int i = 0; i < normals.size(); ++i) {
        
        calculated[i] = false;
        neighbour_edges[i].resize(2); // Only need 2 edges from neighbour
    }

    for (int i = 0; i < faces.size(); ++i) {

        ObjTriangleFace& face = faces[i];

        // Construct Normal
        glm::vec3 verts[] = { vertices[face.vert_indices.x], vertices[face.vert_indices.y], vertices[face.vert_indices.z] };
        glm::vec3 v0_v1 = verts[0] - verts[1];
        glm::vec3 v1_v2 = verts[1] - verts[2];
        glm::vec3 normal = glm::cross(v0_v1, v1_v2);
        
        normals[ face.vert_indices.x ] = normal;
        normals[ face.vert_indices.y ] = normal;
        normals[ face.vert_indices.z ] = normal;

        face.normal_indices = glm::ivec3(face.vert_indices.x, face.vert_indices.y, face.vert_indices.z);
    }

    // Normalize
    for (int i = 0; i < normals.size(); ++i)
        normals[i] = glm::normalize(normals[i]);
}