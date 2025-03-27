#pragma once
#include <glm/glm.hpp>

#include "camera.h"
#include "light_source.h"

class IMesh {
public:
    IMesh() {};
    virtual ~IMesh() {};

    // Load Shader
    virtual void load_shader(const char* vertex_file, const char* fragment_file) = 0;

    // Initializes Mesh with a Model's Data
    virtual void init_model(const char* filename, glm::vec3 color, bool smooth_normals) = 0; 

    // Loads the Triangle Data to the GPU using Vertex Buffer Objects
    // If 'init_model' is not called, 'init' can be called to draw the Default Mesh 
    virtual void init() = 0;

    // Mesh is transformed and rendered on GPU
    virtual void transform_and_render(Camera* camera, bool use_fragment_depth) = 0;

    // Transformations
    virtual void translate(const glm::vec3& translate) = 0;
    virtual void rotate(const float& angle_degrees, const glm::vec3& axis)= 0;
    virtual void scale(const glm::vec3& scale)= 0;
    // virtual void apply_camera(Camera* camera) = 0;
    

    // De-Allocate OpenGL Objects and Resources
    virtual void destroy() = 0;
};
