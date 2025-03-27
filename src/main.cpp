// template based on material from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "objects/mesh.h"
#include "objects/indexed_mesh.h"

#include <unordered_map>

// My ImGui Menus
bool move_mesh = false;
const char* ImGui_mesh_selector();


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST); 



    // My Code 
    //
    // Camera for Perspective Projection
    Camera camera(window, glm::vec2(SCR_WIDTH, SCR_HEIGHT));


    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();// (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // Initialize Backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Shader Paths
    std::string depth_shader[] = { "src/shaders/depth_shader.vert", "src/shaders/depth_shader.frag" }; 
    std::string gouraud_shader[] = { "src/shaders/gouraud_shader.vert", "src/shaders/gouraud_shader.frag" }; 
    std::string phong_shader[] = { "src/shaders/phong_shader.vert", "src/shaders/phong_shader.frag" }; 
    std::string flat_shader[] = { "src/shaders/flat_shader.vert", "src/shaders/flat_shader.frag" }; 
    
    // Shader for Menu
    std::string shaders_names[] = { "Phong", "Gouraud", "Flat", "Depth (Z-Buffer)" };
    std::unordered_map<std::string, std::string[2]> shaders_list;
    shaders_list["Phong"][0] = phong_shader[0];
    shaders_list["Phong"][1] = phong_shader[1];
    shaders_list["Gouraud"][0] = gouraud_shader[0];
    shaders_list["Gouraud"][1] = gouraud_shader[1];
    shaders_list["Flat"][0] = flat_shader[0];
    shaders_list["Flat"][1] = flat_shader[1];
    shaders_list["Depth (Z-Buffer)"][0] = depth_shader[0];
    shaders_list["Depth (Z-Buffer)"][1] = depth_shader[1];
    int current_shader_idx = 0;

    // Meshes
    IndexedMesh cube;
    cube.load_shader(phong_shader[0].c_str(), phong_shader[1].c_str());
    cube.init_model("models/cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), true);
    cube.scale(glm::vec3(607.012756f, 607.012756f, 607.012756f));
    cube.translate(glm::vec3(0.0f, -1.069889f, 0.0f));
    
    IndexedMesh sphere_1;
    sphere_1.load_shader(phong_shader[0].c_str(), phong_shader[1].c_str());
    sphere_1.init_model("models/sphere.obj", glm::vec3(0.0f, 0.0f, 1.0f), true);
    sphere_1.scale(glm::vec3(16.217482f,16.217482f,16.217482f));
    sphere_1.translate(glm::vec3(-1.660879f, -1.620261f, -4.871924f));
    
    IndexedMesh sphere_2;
    sphere_2.load_shader(phong_shader[0].c_str(), phong_shader[1].c_str());
    sphere_2.init_model("models/sphere.obj", glm::vec3(1.0f, 0.0f, 0.0f), true);
    sphere_2.translate(glm::vec3(-7.359352f, -41.284832f, -13.182094));
    
    IndexedMesh pig;
    pig.load_shader(phong_shader[0].c_str(), phong_shader[1].c_str());
    pig.init_model("models/pig.obj", glm::vec3(249.0f / 255.0f, 161.0f / 255.0f, 149.0f / 255.0f), true);
    pig.scale(glm::vec3(0.853205f, 0.853205f,0.853205f));
    pig.translate(glm::vec3(-0.566976f, -48.176361f, 0.670510f));
    
    
    IndexedMesh castle;
    castle.load_shader(phong_shader[0].c_str(), phong_shader[1].c_str());
    castle.init_model("models/castle.obj", glm::vec3(141.0f/255.0f, 78.0f/255.0f, 44.0f/255.0f), true);
    castle.scale(glm::vec3(1.257163f, 1.257163f, 1.257163f));
    castle.translate(glm::vec3(0.0f, -25.862700f, -132.482300f));
    
    IndexedMesh eagle;
    eagle.load_shader(phong_shader[0].c_str(), phong_shader[1].c_str());
    eagle.init_model("models/eagle.obj", glm::vec3(253.0f/255.0f,250.0f/255.0f,114.0f/255.0f), true);
    eagle.scale(glm::vec3(6.445358f, 6.445358f, 6.445358f));
    eagle.translate(glm::vec3(4.585144f, -4.643977, -17.022509f));
    
    // Mesh List
    std::string mesh_names[] = { "Cube", "Red Sphere", "Blue Sphere", "Pig", "Castle", "Eagle" };
    std::unordered_map<std::string, IndexedMesh*> mesh_list; 
    mesh_list["Cube"] = &cube;
    mesh_list["Red Sphere"] = &sphere_2;
    mesh_list["Blue Sphere"] = &sphere_1;
    mesh_list["Pig"] = &pig;
    mesh_list["Castle"] = &castle;
    mesh_list["Eagle"] = &eagle;
    
    
    // mesh_list["Blue Sphere"] = &sphere_1;
    

    // Light Sources
    LightSource light_source_1;
    light_source_1.position = cube.get_local_position();
    light_source_1.model_transform = glm::translate(light_source_1.model_transform, glm::vec3(0.0f, 10.0f, 0.0f));
    light_source_1.apply_transform();

    // Set Up Light
    for (int i = 0; i < mesh_list.size(); ++i) {
        mesh_list[ mesh_names[i] ]->add_light_source(&light_source_1);
    }
    
    //mesh = &pig_mesh;
    // Currently Used Mesh
    int current_mesh_idx = 3;
    IndexedMesh* current_mesh = mesh_list["Pig"];

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Control
    // bool transform_on_gpu = false;
    // bool use_indexed_mesh = false;
    bool im_key_pressed = false;
    bool im_key_currently_pressed = false;
    // Scaling
    float cpu_scale_up_percent = 1.01f;
    float cpu_scale_down_percent = 0.99f;
    float current_scale_up_percent = cpu_scale_up_percent;
    float current_scale_down_percent = cpu_scale_down_percent;
    // Rotate and Translate
    float rotate_factor = 30.0f;
    float translate_factor = 1.0f;
    float current_time, delta_time, last_frame_time;

    // Timing
    bool timing_active = false;
    bool timing_key_pressed = false;
    bool timing_key_currently_pressed = false;
    float cpu_total_time = 0.0f;
    float gpu_total_time = 0.0f;
    float current_run = 0;
    float num_runs = 100;
    
    // Depth Shader
    bool depth_shader_use_fragment_depth = true;
    bool scale_all_mesh = false;
    // Capture Mouse Cursor
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Using Delta Time to Keep Speeds Consistent
        current_time = glfwGetTime();
        delta_time = current_time - last_frame_time;
        last_frame_time = current_time;
        //std::cout << 1.0 / delta_time << "\n";

        // input
        // -----
        processInput(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Process ImGui
        //
        // Mesh Selector
        ImGui::Begin("Mesh Selector");
        if (ImGui::BeginCombo("Select Mesh", mesh_names[current_mesh_idx].c_str()))
        {
            for (int n = 0; n < IM_ARRAYSIZE(mesh_names); n++)
            {
                const bool is_selected = (current_mesh_idx == n);
                if (ImGui::Selectable(mesh_names[n].c_str(), is_selected))
                    current_mesh_idx = n;
            }
            
            current_mesh = mesh_list[mesh_names[current_mesh_idx]];

            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Select Shader", shaders_names[current_shader_idx].c_str()))
        {
            int prev_shader_idx = current_shader_idx;
            for (int n = 0; n < IM_ARRAYSIZE(shaders_names); n++)
            {
                const bool is_selected = (current_shader_idx == n);
                if (ImGui::Selectable(shaders_names[n].c_str(), is_selected))
                    current_shader_idx = n;
            }
            
            if (prev_shader_idx != current_shader_idx) {
                
                // Change Shaders
                std::string shader_vert = shaders_list[ shaders_names[current_shader_idx] ][0];
                std::string shader_frag = shaders_list[ shaders_names[current_shader_idx] ][1];
                for (int i = 0; i < mesh_list.size(); ++i) {

                    mesh_list[ mesh_names[i] ]->load_shader(shader_vert.c_str(), shader_frag.c_str());
                    bool smooth_normals = (shaders_names[current_shader_idx].compare("Flat") != 0);

                    if (mesh_names[i].compare("Cube") == 0) {
                        
                        cube.init_model("models/cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), smooth_normals);
                        // cube.scale(glm::vec3(607.012756f, 607.012756f, 607.012756f));
                        // cube.translate(glm::vec3(0.0f, -1.069889f, 0.0f));
                    } else if (mesh_names[i].compare("Blue Sphere") == 0) {
                    
                        sphere_1.init_model("models/sphere.obj", glm::vec3(0.0f, 0.0f, 1.0f), smooth_normals);
                        // sphere_1.scale(glm::vec3(16.217482f,16.217482f,16.217482f));
                        // sphere_1.translate(glm::vec3(-1.660879f, -1.620261f, -4.871924f));
                    } else if (mesh_names[i].compare("Red Sphere") == 0) {
                        
                        sphere_2.init_model("models/sphere.obj", glm::vec3(1.0f, 0.0f, 0.0f), smooth_normals);
                        //sphere_2.translate(glm::vec3(-7.359352f, -41.284832f, -13.182094));
                    } else if (mesh_names[i].compare("Pig") == 0) {
                    
                        pig.init_model("models/pig.obj", glm::vec3(249.0f / 255.0f, 161.0f / 255.0f, 149.0f / 255.0f), smooth_normals);
                        // pig.scale(glm::vec3(0.853205f, 0.853205f,0.853205f));
                        // pig.translate(glm::vec3(-0.566976f, -48.176361f, 0.670510f));
                    } else if (mesh_names[i].compare("Castle") == 0) {
                    
                        castle.init_model("models/castle.obj", glm::vec3(141.0f/255.0f, 78.0f/255.0f, 44.0f/255.0f), smooth_normals);
                        // castle.scale(glm::vec3(1.257163f, 1.257163f, 1.257163f));
                        // castle.translate(glm::vec3(0.0f, -25.862700f, -132.482300f));
                    } else if (mesh_names[i].compare("Eagle") == 0) {
                        
                        eagle.init_model("models/eagle.obj", glm::vec3(253.0f/255.0f,250.0f/255.0f,114.0f/255.0f), smooth_normals);
                        // eagle.scale(glm::vec3(6.445358f, 6.445358f, 6.445358f));
                        // eagle.translate(glm::vec3(4.585144f, -4.643977, -17.022509f));
                    }
                }
            }
            //current_mesh = mesh_list[mesh_names[current_mesh_idx]];

            ImGui::EndCombo();
        }
        {

            const glm::vec3& translate = current_mesh->get_translate();
            const glm::vec3& scale = current_mesh->get_scale();

            ImGui::SliderFloat("Speed", &translate_factor, 1.0f, 100.0f);
            ImGui::Text("Translate (Keys=W,A,X,D,Q,E): (%f, %f, %f)", translate.x, translate.y, translate.z);
            ImGui::Text("Scale (Left/Right Arrow Keys): (%f, %f, %f)", scale.x, scale.y, scale.z);
            ImGui::Checkbox("Move Mesh (Locks Camera in Position)", &move_mesh);
            ImGui::Checkbox("Scale All Meshes", &scale_all_mesh);
            ImGui::Checkbox("(For Depth Shader) Use z' value instead of World z", &depth_shader_use_fragment_depth);
        }
        ImGui::End();
        
        // Scaling the Mesh
        if (!scale_all_mesh) {
            
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                
                current_mesh->scale(glm::vec3(current_scale_up_percent, current_scale_up_percent, current_scale_up_percent));
            } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                
                current_mesh->scale(glm::vec3(current_scale_down_percent, current_scale_down_percent, current_scale_down_percent));
            }
        } else {
            
            for (int i = 1; i < mesh_list.size(); ++i) {
                
                IndexedMesh* mesh = mesh_list[ mesh_names[i] ];
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                    
                    mesh->scale(glm::vec3(current_scale_up_percent, current_scale_up_percent, current_scale_up_percent));
                } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                    
                    mesh->scale(glm::vec3(current_scale_down_percent, current_scale_down_percent, current_scale_down_percent));
                }
            }
        }
        
        // Update Camera
        if (!move_mesh)
            camera.update(window, delta_time);
        else {
            // Translating the Mesh
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                
                current_mesh->translate(glm::vec3(0.0f, translate_factor * delta_time, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
                
                current_mesh->translate(glm::vec3(0.0f, -translate_factor * delta_time, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                
                current_mesh->translate(glm::vec3(-translate_factor * delta_time, 0.0f, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                
                current_mesh->translate(glm::vec3(translate_factor * delta_time, 0.0f, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                
                current_mesh->translate(glm::vec3(0.0f, 0.0f, -translate_factor * delta_time));
            }
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
                
                current_mesh->translate(glm::vec3(0.0f, 0.0f, translate_factor * delta_time));
            }
        }
        
        // ImGui Menus
        camera.ImGui_Run();
        light_source_1.ImGui_Run(&camera, window, delta_time);
        
        // Call to ImGui EndFrame
        ImGui::Render();
        
        // render
        // ------
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the Mesh
        for (int i = 0; i < mesh_list.size(); ++i) {
            mesh_list[ mesh_names[i] ]->transform_and_render(&camera, depth_shader_use_fragment_depth);
        }  

        // Render ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //current_mesh->destroy();

    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
