#version 330 core
out vec4 o_color;
  
uniform vec3 u_object_color;
uniform vec3 u_light_color;

void main() {
    o_color = vec4(u_light_color * u_object_color, 1.0);
}

