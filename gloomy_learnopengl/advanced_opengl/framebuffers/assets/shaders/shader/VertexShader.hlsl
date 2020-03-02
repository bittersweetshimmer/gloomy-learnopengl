#version 330 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texcoords;

out vec2 f_texcoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    f_texcoords = v_texcoords;

    gl_Position = u_projection * u_view * u_model * vec4(v_position, 1.0);
}