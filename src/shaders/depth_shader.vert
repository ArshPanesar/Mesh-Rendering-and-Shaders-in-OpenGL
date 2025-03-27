#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

uniform mat4 view_mat;
uniform mat4 proj_mat;
uniform mat4 model_mat;

out vec4 vertPos;
out vec3 outColor;

void main()
{
   gl_Position = proj_mat * view_mat * model_mat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   vertPos = model_mat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   outColor = color;
}