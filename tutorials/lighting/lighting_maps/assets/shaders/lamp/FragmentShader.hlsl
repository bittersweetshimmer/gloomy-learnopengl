#version 330 core
out vec4 o_color;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_light;

void main() {
    o_color = vec4(u_light.ambient, 1.0);
}

