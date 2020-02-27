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

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define POINT_LIGHTS_N 4

uniform DirectionalLight u_directional_light;
uniform PointLight u_point_lights[POINT_LIGHTS_N];
uniform SpotLight u_spot_light;
uniform Material u_material;

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 fragment_position, vec3 view_direction);

void main() {
    vec3 norm = normalize(f_normal);
    vec3 view_direction = normalize(u_view_position - f_position);
    
    vec3 result = calc_directional_light(u_directional_light, norm, view_direction);
   /* for(int i = 0; i < POINT_LIGHTS_N; i++)
        result += calc_point_light(u_point_lights[i], norm, f_position, view_direction);    */
    result += calc_spot_light(u_spot_light, norm, f_position, view_direction);    
    
    o_color = vec4(result, 1.0);
}

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction) {
    vec3 light_direction = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, f_texcoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, f_texcoords));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, f_texcoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fragment_position);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    // attenuation
    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, f_texcoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, f_texcoords));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, f_texcoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 fragment_position, vec3 view_direction) {
    vec3 light_direction = normalize(light.position - fragment_position);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    // attenuation
    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(light_direction, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, f_texcoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, f_texcoords));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, f_texcoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}