#version 330 core
out vec4 o_color;

in vec3 f_normal;
in vec3 f_position;

uniform vec3 u_light_position;
uniform vec3 u_view_position;
uniform vec3 u_object_color;
uniform vec3 u_light_color;

void main() {
    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * u_light_color;
  	
    // diffuse 
    vec3 normal = normalize(f_normal);
    vec3 light_direction = normalize(u_light_position - f_position);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * u_light_color;
    
    // specular
    float specular_strength = 0.5;
    vec3 view_direction = normalize(u_view_position - f_position);
    vec3 reflect_direction = reflect(-light_direction, normal);  
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * spec * u_light_color;  
        
    vec3 result = (ambient + diffuse + specular) * u_object_color;
    o_color = vec4(result, 1.0);
}