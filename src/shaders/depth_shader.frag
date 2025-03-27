#version 330 core

uniform vec2 near_far_dists;
uniform bool use_fragment_depth;

in vec4 vertPos;
in vec3 outColor;
out vec4 FragColor;

float near = 0.1;
float far = 10.0;

// gl_FragCoord.z grows non-linearly: For Small Values it grows rapidly and for Large Values it grows too slowly
// We need to make its growth Linear so that visualization is possible
float linear_growth(float depth) {

   float z = depth * 2.0 - 1.0; // back to NDC 
   return (2.0 * near_far_dists.x * near_far_dists.y) / (near_far_dists.y + near_far_dists.x - z * (near_far_dists.y - near_far_dists.x));	
}

void main()
{
   if (use_fragment_depth) {

      float linear_z = linear_growth(gl_FragCoord.z) / near_far_dists.y; //
      FragColor = vec4(vec3(linear_z), 1.0); //vertPos.z / near_far_dists.y
   } else {

      //float linear_z = linear_growth(abs(vertPos.z)) / near_far_dists.y;
      FragColor = vec4(vec3(abs(vertPos.z) / near_far_dists.y), 1.0);
   }
}