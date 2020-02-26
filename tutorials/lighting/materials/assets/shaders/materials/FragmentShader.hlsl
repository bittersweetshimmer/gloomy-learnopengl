#version 330 core
out vec4 o_color;

in vec3 f_normal;
in vec3 f_position;

uniform vec3 u_view_position;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 ambient = u_material.ambient * u_light.ambient;
  	
    // diffuse 
    vec3 normal = normalize(f_normal);
    vec3 light_direction = normalize(u_light.position - f_position);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = u_material.diffuse * diff * u_light.diffuse;
    
    // specular
    vec3 view_direction = normalize(u_view_position - f_position);
    vec3 reflect_direction = reflect(-light_direction, normal);  
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    vec3 specular = u_material.diffuse * spec * u_light.specular;  
        
    vec3 result = (ambient + diffuse + specular);
    o_color = vec4(result, 1.0);
}