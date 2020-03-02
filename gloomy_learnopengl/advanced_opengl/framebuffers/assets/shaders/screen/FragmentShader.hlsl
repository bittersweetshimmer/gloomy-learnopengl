#version 330 core
out vec4 o_color;

in vec2 f_texcoords;

uniform sampler2D u_screen_texture;

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(u_screen_texture, f_texcoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);

    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    o_color = vec4(col, 1.0);
}