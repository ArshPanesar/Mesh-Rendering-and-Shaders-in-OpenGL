#include "mesh.h"

Mesh::Mesh() : 
    VAO(0),
    VBO(0),
    num_vertices(0),
    triangles(),
    shader(),
    transform_mat(1.0f)     {

    triangles.push_back(std::make_unique<Triangle>());
    triangles.push_back(std::make_unique<Triangle>());

    Triangle& t1 = *triangles[0];
    t1.vertices[0].position = glm::vec3(0.5f, 0.05f, 0.0f);
    t1.vertices[1].position = glm::vec3(-0.5f, 0.05f, 0.0f);
    t1.vertices[2].position = glm::vec3(0.0f, 0.8f, 0.0f);
    t1.vertices[0].color = glm::vec3(1.0f, 0.0, 0.0f);
    t1.vertices[1].color = glm::vec3(0.0f, 1.0f, 0.0f);
    t1.vertices[2].color = glm::vec3(0.0f,  0.0f, 1.0f);

    Triangle& t2 = *triangles[1];
    t2.vertices[0].position = glm::vec3(0.5f, -0.05f, 0.0f);
    t2.vertices[1].position = glm::vec3(-0.5f, -0.05f, 0.0f);
    t2.vertices[2].position = glm::vec3(0.0f, -0.8f, 0.0f);
    t2.vertices[0].color = glm::vec3(1.0f, 1.0, 0.0f);
    t2.vertices[1].color = glm::vec3(0.0f, 1.0f, 1.0f);
    t2.vertices[2].color = glm::vec3(1.0f,  0.0f, 1.0f);
}

Mesh::~Mesh() {

    destroy();
}

void Mesh::load_shader(const char* vertex_file, const char* fragment_file) {

    shader.load(vertex_file, fragment_file);
}


void Mesh::init_model(const char* filename, glm::vec3 color, bool smooth_normals) {

    // Load Model Data
    ObjLoader obj_loader;
    if (!obj_loader.load(filename))
        return;

    // Make Triangles out of the Model Data
    triangles.clear();

    int num_triangles = obj_loader.faces.size();
    for (int i = 0; i < num_triangles; ++i) {

        triangles.push_back(std::make_unique<Triangle>());

        triangles[i]->vertices[0].position = obj_loader.vertices[ obj_loader.faces[i].vert_indices.x ];
        triangles[i]->vertices[1].position = obj_loader.vertices[ obj_loader.faces[i].vert_indices.y ];
        triangles[i]->vertices[2].position = obj_loader.vertices[ obj_loader.faces[i].vert_indices.z ];        

        triangles[i]->vertices[0].color = glm::vec3(1.0f, 1.0f, 0.0f);
        triangles[i]->vertices[1].color = glm::vec3(0.0f, 1.0f, 1.0f);
        triangles[i]->vertices[2].color = glm::vec3(1.0f, 0.0f, 1.0f);
    }

    // Initialize with OpenGL
    init();

    // Debug Print Name and #Vertices
    std::cout << "Separate Triangles"  << "\n";
    std::cout << "Mesh Initialized with Model: " << filename << "\n";
    std::cout << "Number of Vertices: " << triangles.size() * 3 << "\n";
    std::cout << "Memory Usage: " << triangles.size() * sizeof(Triangle) << "\n";   
}

void Mesh::init() {

    // Destroy Mesh, if it was already Initialized before
    destroy();

    // Generate the Vertex Array and the Buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO to make it current
    glBindVertexArray(VAO);
    
    // Copy the Triangles Data to a Temporary Buffer
    //
    num_vertices = triangles.size() * 3;
    const int num_vert_fields = sizeof(Vertex) / sizeof(float); // Number of Float Fields per Vertex    
    const int vertices_size = num_vertices * num_vert_fields;
    float* vertices = new float[vertices_size];
    for (int i = 0; i < triangles.size(); ++i) {

        Triangle& tref = *triangles[i];
        int vi = i * num_vert_fields * 3;
        for (int j = 0; j < 3; ++j) {
            
            vertices[vi + 0] = tref.vertices[j].position.x;
            vertices[vi + 1] = tref.vertices[j].position.y;
            vertices[vi + 2] = tref.vertices[j].position.z;
            vertices[vi + 3] = tref.vertices[j].color.r;
            vertices[vi + 4] = tref.vertices[j].color.g;
            vertices[vi + 5] = tref.vertices[j].color.b;

            vi += num_vert_fields;
        }
    }

    // Load Temporary Buffer Data to Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
  
    // De-Allocate Temporary Buffer
    delete[] vertices;

    // Set Attributes of Vertices
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void Mesh::render() {
    
    shader.use();

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glBindVertexArray(0); // unbind our VA no need to unbind it every time 
}

void Mesh::transform_and_render(Camera* camera, bool use_fragment_depth) {

    shader.use();

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    shader.set_uniform_mat4("model_view_mat", transform_mat);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glBindVertexArray(0); // unbind our VA no need to unbind it every time 
}

void Mesh::translate(const glm::vec3& translate) {

    transform_mat = glm::translate(transform_mat, translate);
}

void Mesh::rotate(const float& angle_degrees, const glm::vec3& axis) {

    transform_mat = glm::rotate(transform_mat, glm::radians(angle_degrees), axis);
}

void Mesh::scale(const glm::vec3& scale) {

    transform_mat = glm::scale(transform_mat, scale);
}

void Mesh::compute_transform() {

    for (int i = 0; i < triangles.size(); ++i) {
    
        triangles[i]->vertices[0].position = transform_mat * glm::vec4(triangles[i]->vertices[0].position, 1.0f);
        triangles[i]->vertices[1].position = transform_mat * glm::vec4(triangles[i]->vertices[1].position, 1.0f);
        triangles[i]->vertices[2].position = transform_mat * glm::vec4(triangles[i]->vertices[2].position, 1.0f);
    }

    transform_mat = glm::mat4(1.0f);
    init();
}

void Mesh::compute_transform_gpu() {

    shader.set_uniform_mat4("model_view_mat", transform_mat);
}

void Mesh::destroy() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    VAO = 0;
    VBO = 0;
}