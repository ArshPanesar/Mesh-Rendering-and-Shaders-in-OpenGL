#include "camera.h"

// Mouse Parameters
float mouse_pos_x = 0.0f;
float mouse_pos_y = 0.0f;    
float mouse_offset_x = 0.0f;    
float mouse_offset_y = 0.0f;    
float mouse_speed = 0.1f;
bool first_call = true;
bool mouse_active = false;

// Mouse Angles
float yaw = -90.0f;
float pitch = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera::Camera(GLFWwindow* window, glm::vec2 screen_dim) :
    look_direction(0.0f, 0.0f, -1.0f),
    screen_res(screen_dim) {

    // Set up the View Matrix    
    view = glm::mat4(1.0f);
    position = glm::vec3(-0.855185f, -40.196606f, 8.753914f);

    // Set Up the Projection Matrix
    near_far_vec = glm::vec2(0.1f, 200.0f);
    perspective_projection = glm::perspective(glm::radians(45.0f), screen_res.x / screen_res.y, near_far_vec.x, near_far_vec.y);

    // Mouse Movement Setup
    mouse_pos_x = 800.0f / 2.0f;
    mouse_pos_y = 600.0f / 2.0f;

    glfwSetCursorPosCallback(window, mouse_callback); 
}

Camera::~Camera() {

}

void Camera::update(GLFWwindow* window, float delta_time) {

    if (lock_update)
        return;
    
    // Translation
    camera_right_direction = glm::normalize(glm::cross(look_direction, up_direction));
    camera_up_direction = glm::normalize(glm::cross(look_direction, camera_right_direction));
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)    
        position += translate_speed * delta_time * look_direction;
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)    
        position += -translate_speed * delta_time * look_direction;
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)    
        position += -translate_speed * delta_time * camera_right_direction;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)    
        position += translate_speed * delta_time * camera_right_direction;
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)    
        position += -translate_speed * delta_time * camera_up_direction;
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)    
        position += translate_speed * delta_time * camera_up_direction;

    // Rotation
    glm::vec3 new_direction;
    new_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_direction.y = sin(glm::radians(pitch));
    new_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    look_direction = glm::normalize(new_direction);  
    
    // Capture Mouse Cursor
    mouse_active = glfwGetMouseButton(window, 1) == GLFW_PRESS;
    if (mouse_active) {    

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    } else {
        
        first_call = true;        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    
    }


    // Update the View Matrix
    view = glm::lookAt(position, position + look_direction, up_direction);
}

void Camera::ImGui_Run() {

    // ImGui Stuff
    static float fov_angle_deg = 45.0f;
    static float near_plane = 0.1f;
    static float far_plane = 200.0f;
    static float res[] = { screen_res.x, screen_res.y };
    
    
    ImGui::Begin("Perspective Projection Parameters");

    if (ImGui::TreeNode("View Parameters (Alter with Keyboard/Mouse)")) {
        
        ImGui::Text("Position (Keys=W,A,X,D,Q,E): (%f, %f, %f)", position.x, position.y, position.z);
        ImGui::Text("LookAt Vector (Hold Right Click and Move Mouse): (%f, %f, %f)", look_direction.x, look_direction.y, look_direction.z);
        ImGui::Text("Right Vector: (%f, %f, %f)", camera_right_direction.x, camera_right_direction.y, camera_right_direction.z);
        ImGui::Text("Up Vector: (%f, %f, %f)", camera_up_direction.x, camera_up_direction.y, camera_up_direction.z);
        ImGui::TreePop();
    }

    ImGui::NewLine();
    
    if (ImGui::TreeNode("Perspective Parameters")) {
        
        ImGui::SliderFloat2("View Resolution", res, 100.0f, 4000.0f);
        ImGui::SliderFloat("Field Of View (FOV)", &fov_angle_deg, 0.0f, 180.0f);
        ImGui::SliderFloat("Near Plane Distance", &near_plane, 0.1f, 1.0f);
        ImGui::SliderFloat("Far Plane Distance", &far_plane, 10.0f, 2000.0f);
        
        perspective_projection = glm::perspective(glm::radians(fov_angle_deg), res[0] / res[1], near_plane, far_plane);
        screen_res.x = res[0];
        screen_res.y = res[1];
        near_far_vec.x = near_plane;
        near_far_vec.y = far_plane;

        ImGui::TreePop();
    }

    ImGui::SliderFloat("Speed", &translate_speed, 1.0f, 100.0f);
    ImGui::End();
}

// Mouse Callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    float last_pos_x = mouse_pos_x;
    float last_pos_y = mouse_pos_y;
    
    // Constraint for First Time this function is called
    if (first_call) {

        last_pos_x = xpos;
        last_pos_y = ypos;
        first_call = false;
    }

    if (!mouse_active)
        return;

    // Calculate how much the mouse has moved since the last frame
    mouse_offset_x = xpos - last_pos_x;
    mouse_offset_y = last_pos_y - ypos;
    mouse_pos_x = xpos;
    mouse_pos_y = ypos;

    // Apply Sensitivity
    mouse_offset_x *= mouse_speed;
    mouse_offset_y *= mouse_speed;

    // Calculate the Angles
    yaw += mouse_offset_x;
    pitch += mouse_offset_y;

    // Clamp the Angles so that View does not Flip
    pitch = (pitch > 89.0f) ? 89.0f : ( (pitch < -89.0f) ? -89.0f : pitch );
}
