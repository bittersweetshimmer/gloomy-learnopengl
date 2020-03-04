#version 330 core
out vec4 o_color;

in vec2 f_texcoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

uniform Material u_material;

void main() {
    o_color = texture(u_material.diffuse, f_texcoords);
}