#include "shader.h"

Shader::Shader() :
    program(0)    {

}

Shader::~Shader() {

    glDeleteProgram(program);
    program = 0;
}

void Shader::load(const char* vertex_file, const char* frag_file) {

    std::string vertex_source, fragment_source;

    // Read from File
    std::ifstream file_reader;

    // Read Vertex Shader
    file_reader.open(vertex_file);
    if (!file_reader.is_open()) {
        
        std::cout << "ERROR: Cannot Open Vertex Shader File at Path " << vertex_file << "\n";
        return;
    }
    std::stringstream sstream;
    sstream << file_reader.rdbuf();
    vertex_source = sstream.str();
    file_reader.close();

    // Read Fragment Shader
    file_reader.open(frag_file);
    if (!file_reader.is_open()) {
        
        std::cout << "ERROR: Cannot Open Fragment Shader File at Path " << vertex_file << "\n";
        return;
    }
    std::stringstream strstream;
    strstream << file_reader.rdbuf();
    fragment_source = strstream.str();
    file_reader.close();
    
    // We now have the Source Code from Files
    const char* cvertex_source = vertex_source.c_str();
    const char* cfragment_source = fragment_source.c_str();

    // Compile Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &cvertex_source, NULL);
    glCompileShader(vertexShader);

    // Check for Compilation Errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: Vertex Shader Compilation Failed\n" << infoLog << std::endl;
    }

    // Compile Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &cfragment_source, NULL);
    glCompileShader(fragmentShader);

    // Check for Compilation Errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
    }

    // Link Shaders
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // Check for Linking Errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR: Linking Failed.\n" << infoLog << std::endl;
    }

    // Delete the Shaders6 as Program has been created
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
