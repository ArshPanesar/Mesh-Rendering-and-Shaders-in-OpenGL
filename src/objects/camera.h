#pragma once
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <iostream>
// #include "triangle.h"
// #include "../shaders/shader.h"
// #include "obj_loader.h"

class Camera {
private:

    glm::vec3 position;
    glm::vec3 camera_right_direction;
    glm::vec3 camera_up_direction;
    glm::vec3 up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 look_direction = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec2 screen_res;

    glm::mat4 view;
    glm::mat4 perspective_projection;

    float translate_speed = 2.0f;
    glm::vec2 near_far_vec;

    bool lock_update = false;

public:
    Camera(GLFWwindow* window, glm::vec2 screen_dim);
    ~Camera();

    void update(GLFWwindow* window, float delta_time);
    void ImGui_Run();

    void set_lock_update(bool val) { lock_update = val; }

    inline const glm::vec3& get_position() const { return position; }

    inline const glm::mat4& get_view_mat() const { return view; }
    inline const glm::mat4& get_proj_mat() const { return perspective_projection; }

    inline const glm::vec2& get_near_and_far_planes() const { return near_far_vec; };
};
