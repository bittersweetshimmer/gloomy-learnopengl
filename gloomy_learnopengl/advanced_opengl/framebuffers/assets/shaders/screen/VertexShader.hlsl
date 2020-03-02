#version 330 core
layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_texcoords;

out vec2 f_texcoords;

void main() {
    f_texcoords = v_texcoords;
    gl_Position = vec4(v_position.x, v_position.y, 0.0, 1.0); 
}  