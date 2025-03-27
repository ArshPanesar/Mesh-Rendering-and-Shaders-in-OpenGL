#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "triangle.h"
#include "../shaders/shader.h"
#include "obj_loader.h"

#include "imesh.h"

class Mesh : public IMesh {
private:

    GLuint VAO;
    GLuint VBO;    

    GLuint num_vertices;

    std::vector<std::unique_ptr<Triangle>> triangles;

    Shader shader;

    glm::mat4 transform_mat;

public:
    Mesh();
    ~Mesh();

    // Load Shader
    void load_shader(const char* vertex_file, const char* fragment_file);

    // Initializes Mesh with a Model's Data
    void init_model(const char* filename, glm::vec3 color, bool smooth_normals); 

    // Loads the Triangle Data to the GPU using Vertex Buffer Objects
    // If 'init_model' is not called, 'init' can be called to draw the Default Mesh 
    void init();

    // Draws the Mesh
    void render();
    // Mesh is transformed and rendered on GPU
    void transform_and_render(Camera* camera, bool use_fragment_depth);

    // Transformations
    void translate(const glm::vec3& translate);
    void rotate(const float& angle_degrees, const glm::vec3& axis);
    void scale(const glm::vec3& scale);
    void compute_transform();
    void compute_transform_gpu();


    // De-Allocate OpenGL Objects and Resources
    void destroy();
};
