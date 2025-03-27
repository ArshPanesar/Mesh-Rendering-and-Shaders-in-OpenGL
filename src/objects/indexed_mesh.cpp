#include "indexed_mesh.h"

IndexedMesh::IndexedMesh() : 
    VAO(0),
    EBO(0),
    vertices(),
    faces(),
    num_vertices(0),
    light_source(nullptr),
    shader(),
    transform_mat(1.0f)     {

    // Rectangle
    Vertex v1, v2, v3, v4;
    // Fill Position and Color
    v1.position = glm::vec3(-0.5f, 0.5f, 0.0f);
    v2.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    v3.position = glm::vec3(0.5f, -0.5f, 0.0f);
    v4.position = glm::vec3(0.5f, 0.5f, 0.0f);
    
    v1.color = glm::vec3(1.0f, 1.0, 0.0f);
    v2.color = glm::vec3(0.0f, 1.0f, 1.0f);
    v3.color = glm::vec3(1.0f,  0.0f, 1.0f);
    v4.color = glm::vec3(1.0f, 1.0, 0.0f);

    // Add to Vertex List
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    // Providing Indices in Anti-Clockwise Order
    faces.push_back(glm::uvec3(0, 1, 2));
    faces.push_back(glm::uvec3(0, 2, 3));
}

IndexedMesh::~IndexedMesh() {

    destroy();
}

void IndexedMesh::load_shader(const char* vertex_file, const char* fragment_file) {

    shader.load(vertex_file, fragment_file);
}


void IndexedMesh::init_model(const char* filename, glm::vec3 color, bool smooth_normals) {

    // Load Model Data
    ObjLoader obj_loader;
    if (!obj_loader.load(filename))
        return;

    if (smooth_normals)
        obj_loader.generate_smooth_normals();
    else
        obj_loader.generate_flat_normals();

    // Make Triangles out of the Model Data
    vertices.clear();
    faces.clear();

    // Copy Vertices and Add Color
    glm::vec3 colors[] = { glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f) };
    for (int i = 0; i < obj_loader.vertices.size(); ++i) {

        Vertex v;
        v.position = obj_loader.vertices[i];
        v.color = color;

        vertices.push_back(v);
    }

    // Copy Face Data
    for (int i = 0; i < obj_loader.faces.size(); ++i) {

        glm::ivec3 vert_indices = obj_loader.faces[i].vert_indices;
        glm::ivec3 normal_indices = obj_loader.faces[i].normal_indices;

        // Add Vertex Indices to the Face List
        faces.push_back(vert_indices);

        // Add Normal Vector to the Vertices
        if (obj_loader.normals.size() > 0) {
        
            vertices[ vert_indices.x ].normal = obj_loader.normals[ normal_indices.x ];
            vertices[ vert_indices.y ].normal = obj_loader.normals[ normal_indices.y ];
            vertices[ vert_indices.z ].normal = obj_loader.normals[ normal_indices.z ];
        }
    }

    // Initialize with OpenGL
    init();

    // Calculate the Center Local Coordinates
    local_position = vertices[0].position; // Just grabbing a vertex for now

    // Debug Print Name and #Vertices
    std::cout << "Indexed Triangles"  << "\n";
    std::cout << "Mesh Initialized with Model: " << filename << "\n";
    std::cout << "Local Position: " << local_position.x << ", " << local_position.y << ", " << local_position.z << "\n";
    std::cout << "Number of Vertices: " << vertices.size() << "\n";
    std::cout << "Memory Usage: " << faces.size() * sizeof(unsigned int) * 3 << " Bytes\n";
}

void IndexedMesh::init() {

    // Destroy Mesh, if it was already Initialized before
    destroy();

    // Generate the Vertex Array and the Buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO to make it current
    glBindVertexArray(VAO);
    
    // Copy the Vertices and Indices Data to Temporary Buffers
    num_vertices = vertices.size();
    int num_indices = faces.size() * 3;
    const int vertices_size = num_vertices * 9;
    // Copy Vertices
    float* vertices_buffer = new float[vertices_size];
    for (int i = 0; i < num_vertices; ++i) {
        
        int j = i * 9;
        vertices_buffer[j + 0] = vertices[i].position.x;
        vertices_buffer[j + 1] = vertices[i].position.y;
        vertices_buffer[j + 2] = vertices[i].position.z;
        
        vertices_buffer[j + 3] = vertices[i].color.r;
        vertices_buffer[j + 4] = vertices[i].color.g;
        vertices_buffer[j + 5] = vertices[i].color.b;

        vertices_buffer[j + 6] = vertices[i].normal.x;
        vertices_buffer[j + 7] = vertices[i].normal.y;
        vertices_buffer[j + 8] = vertices[i].normal.z;
    }
    // Copy Indices
    GLuint* indices_buffer = new GLuint[num_indices];
    for (int i = 0; i < faces.size(); ++i) {

        int j = i * 3;
        indices_buffer[j + 0] = faces[i].x;
        indices_buffer[j + 1] = faces[i].y;
        indices_buffer[j + 2] = faces[i].z;
    }

    // Load Vertices Buffer Data to the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_size, vertices_buffer, GL_STATIC_DRAW);
    
    // Load Indices Buffer Data to Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_indices, indices_buffer, GL_STATIC_DRAW);

    // De-Allocate Temporary Buffers
    delete[] vertices_buffer;
    delete[] indices_buffer;

    // Set Attributes of Vertices
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

    // Unbind EBO, VBO and VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

    num_vertices = faces.size() * 3;
}

void IndexedMesh::add_light_source(LightSource* source) {
    
    light_source = source;
}

void IndexedMesh::transform_and_render(Camera* camera, bool use_fragment_depth) {

    shader.use();

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    
    shader.set_uniform_mat4("model_mat", transform_mat);
    shader.set_uniform_mat4("view_mat", camera->get_view_mat());
    shader.set_uniform_mat4("proj_mat", camera->get_proj_mat());
    shader.set_uniform_vec2("near_far_dists", camera->get_near_and_far_planes());
    shader.set_uniform_bool("use_fragment_depth", use_fragment_depth);
    if (light_source != nullptr) {
     
        shader.set_uniform_vec3("light_position", light_source->position);
        shader.set_uniform_vec3("eye_position", camera->get_position());
    }
    glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0); // unbind our VA no need to unbind it every time 
}

void IndexedMesh::translate(const glm::vec3& translate) {

    transform_mat = glm::translate(transform_mat, translate);
    translate_tracker += translate;
}

void IndexedMesh::rotate(const float& angle_degrees, const glm::vec3& axis) {

    transform_mat = glm::rotate(transform_mat, glm::radians(angle_degrees), axis);
}

void IndexedMesh::scale(const glm::vec3& scale) {

    transform_mat = glm::scale(transform_mat, scale);

    glm::mat4 scale_tracker_transform = glm::mat4(1.0f);
    scale_tracker_transform = glm::scale(scale_tracker_transform, scale);
    scale_tracker = scale_tracker_transform * glm::vec4(scale_tracker, 1.0f);
}

// void IndexedMesh::compute_transform_gpu() {

//     shader.set_uniform_mat4("model_view_mat", transform_mat);
// }

void IndexedMesh::destroy() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);

    VAO = 0;
    VBO = 0;
    EBO = 0;
}

const glm::vec3 IndexedMesh::get_local_position() const {
    return local_position;
}