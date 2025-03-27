#version 330 core

uniform vec3 light_position;
uniform vec3 eye_position;

in vec3 outFragPos;
in vec3 outNormal;
in vec3 outColor;
out vec4 FragColor;

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

    // Gourard Shading
    vec3 illuminated_color = vec3(0.0, 0.0, 0.0);
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    illuminated_color += ambient_lighting(0.5, light_color);
    illuminated_color += diffuse_lighting(light_position, outFragPos, outNormal, 1.0, light_color);
    illuminated_color += specular_lighting(light_position, eye_position, outFragPos, outNormal, 32.0, 200.0, light_color);
    illuminated_color *= (outColor + vec3(0.01)); // Add a little offset for specular shiny
    
    FragColor = vec4(illuminated_color, 1.0);
}
