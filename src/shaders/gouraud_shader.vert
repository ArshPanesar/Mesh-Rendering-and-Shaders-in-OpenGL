#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

uniform mat4 view_mat;
uniform mat4 proj_mat;
uniform mat4 model_mat;
uniform vec3 light_position;
uniform vec3 eye_position;

out vec3 outColor;

vec3 ambient_lighting(float intensity, vec3 color) {

   return intensity * color;
}

vec3 diffuse_lighting(vec3 light_pos, vec3 vert_pos, vec3 normal, float intensity, vec3 color) {
   
   vec3 vl = normalize(light_pos - vert_pos);
   float max_component = max( 0.0, dot(normal, vl) );
   return intensity * max_component * color;
}

vec3 specular_lighting(vec3 light_pos, vec3 eye_pos, vec3 vert_pos, vec3 normal, float power, float intensity, vec3 color) {
   
   vec3 ve = normalize(eye_pos - vert_pos);
   vec3 vl = normalize(light_pos - vert_pos);
   vec3 vr = (2.0 * dot(normal, vl)) * normal - vl;
   vr = normalize(vr);

   float max_component = max(0.0, dot(ve, vr));
   float specular = pow(max_component, power);
   return intensity * specular * color;
}

void main()
{
   vec4 model_homo_pos = model_mat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   gl_Position = proj_mat * view_mat * model_homo_pos;
   vec3 model_pos = vec3(model_homo_pos.x, model_homo_pos.y, model_homo_pos.z);

   // Gourard Shading
   vec3 illuminated_color = vec3(0.0, 0.0, 0.0);
   vec3 light_color = vec3(1.0, 1.0, 1.0);

   illuminated_color += ambient_lighting(0.5, light_color);
   illuminated_color += diffuse_lighting(light_position, model_pos, normal, 1.0, light_color);
   illuminated_color += specular_lighting(light_position, eye_position, model_pos, normal, 32.0, 200.0, light_color);
   illuminated_color *= (color + vec3(0.01)); // Add a little offset for specular shiny

   outColor = illuminated_color;
}