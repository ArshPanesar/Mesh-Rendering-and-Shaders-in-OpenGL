#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "camera.h"

class LightSource {
public:
    // Position
    glm::vec3 position;

    // Model Transformation
    glm::mat4 model_transform = glm::mat4(1.0f);

    // ImGui Stuff
    bool move = false;
    float speed = 10.0f;

public:

    void ImGui_Run(Camera* camera, GLFWwindow* window, float delta_time) {

        ImGui::Begin("Light");
        ImGui::Checkbox("Move Light (Locks Camera in Position)", &move);
        ImGui::Text("Position: (%f, %f, %f)", position.x, position.y, position.z);
        ImGui::SliderFloat("Speed", &speed, 10.0f, 100.0f);
        ImGui::End();

        camera->set_lock_update(move);

        if (!move)
            return;

        // Translating the Mesh
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            
            model_transform = glm::translate(model_transform, glm::vec3(0.0f, speed * delta_time, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            
            model_transform = glm::translate(model_transform, glm::vec3(0.0f, -speed * delta_time, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            
            model_transform = glm::translate(model_transform, glm::vec3(-speed * delta_time, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            
            model_transform = glm::translate(model_transform, glm::vec3(speed * delta_time, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            
            model_transform = glm::translate(model_transform, glm::vec3(0.0f, 0.0f, -speed * delta_time));
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            
            model_transform = glm::translate(model_transform, glm::vec3(0.0f, 0.0f, speed * delta_time));
        }
        apply_transform();
    }

    void apply_transform() {

        position = model_transform * glm::vec4(position, 1.0f);
        model_transform = glm::mat4(1.0f);
    }
}; 