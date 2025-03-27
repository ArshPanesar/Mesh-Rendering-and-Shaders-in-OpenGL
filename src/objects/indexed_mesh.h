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
#include <GL/gl.h>

#include "imesh.h"

class IndexedMesh : public IMesh {
private:

    GLuint VAO;

    GLuint VBO;
    GLuint EBO;    

    GLuint num_vertices;

    // Indexed Triangle Data Structure
    std::vector<Vertex> vertices;
    std::vector<glm::ivec3> faces;

    // Center Position of this Mesh in Local Space
    glm::vec3 local_position;

    Shader shader;

    glm::mat4 transform_mat;

    LightSource* light_source;

    // Transform Trackers
    glm::vec3 scale_tracker = glm::vec3(1.0f);
    glm::vec3 translate_tracker = glm::vec3(0.0f);

public:
    IndexedMesh();
    ~IndexedMesh();

    // Load Shader
    void load_shader(const char* vertex_file, const char* fragment_file);

    // Initializes Mesh with a Model's Data
    void init_model(const char* filename, glm::vec3 color, bool smooth_normals); 

    // Loads the Triangle Data to the GPU using Vertex Buffer Objects
    // If 'init_model' is not called, 'init' can be called to draw the Default Mesh 
    void init();

    // Set Up a Light Source
    void add_light_source(LightSource* source);

    // Mesh is transformed and rendered on GPU
    void transform_and_render(Camera* camera, bool use_fragment_depth);

    // Transformations
    void translate(const glm::vec3& translate);
    void rotate(const float& angle_degrees, const glm::vec3& axis);
    void scale(const glm::vec3& scale);

    // De-Allocate OpenGL Objects and Resources
    void destroy();

    // Getters
    const glm::vec3 get_local_position() const;
    inline const glm::vec3& get_translate() const { return translate_tracker; }
    inline const glm::vec3& get_scale() const { return scale_tracker; }
};
