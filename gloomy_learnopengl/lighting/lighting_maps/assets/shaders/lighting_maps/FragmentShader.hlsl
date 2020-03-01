#version 330 core
out vec4 o_color;

in vec3 f_normal;
in vec3 f_position;
in vec2 f_texcoords;

uniform vec3 u_view_position;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
uniform Material u_material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_light;

void main() {
    // ambient
    vec3 ambient = texture(u_material.diffuse, f_texcoords).rgb  * u_light.ambient;
  	
    // diffuse 
    vec3 normal = normalize(f_normal);
    vec3 light_direction = normalize(u_light.position - f_position);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = texture(u_material.diffuse, f_texcoords).rgb * diff * u_light.diffuse;
    
    // specular
    vec3 view_direction = normalize(u_view_position - f_position);
    vec3 reflect_direction = reflect(-light_direction, normal);  
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    vec3 specular = texture(u_material.specular, f_texcoords).rgb * spec * u_light.specular;  
        
    vec3 result = (ambient + diffuse + specular);
    o_color = vec4(result, 1.0);
}