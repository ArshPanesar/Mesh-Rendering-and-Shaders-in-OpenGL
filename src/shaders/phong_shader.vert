#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

uniform mat4 view_mat;
uniform mat4 proj_mat;
uniform mat4 model_mat;

out vec3 outFragPos;
out vec3 outNormal;
out vec3 outColor;

void main()
{
    vec4 model_homo_pos = model_mat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    gl_Position = proj_mat * view_mat * model_homo_pos;
    vec3 model_pos = vec3(model_homo_pos.x, model_homo_pos.y, model_homo_pos.z);

    outFragPos = model_pos;
    outNormal = normal;
    outColor = color;
}