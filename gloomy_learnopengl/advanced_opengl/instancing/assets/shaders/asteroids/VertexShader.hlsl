#version 330 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoords;
layout (location = 3) in mat4 v_instance_matrix;

out vec2 f_texcoords;

uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    f_texcoords = v_texcoords;

    gl_Position = u_projection * u_view * v_instance_matrix * vec4(v_position, 1.0);
}