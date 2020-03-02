#version 330 core
out vec4 o_color;

in vec2 f_texcoords;

uniform sampler2D u_texture;

void main() {
    o_color = texture(u_texture, f_texcoords);
}