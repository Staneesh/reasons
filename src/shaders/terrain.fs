#version 330 core
out vec4 FragColor;

in vec4 pos;
in vec3 world_pos;
in vec3 normal;

uniform vec3 camera_pos;

vec3 light_pos = vec3(0.0f, 2.0f, 0.0f);
vec3 terrain_color = vec3(0.2f, 0.8f, 0.1f);
vec3 light_color = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;
  	
    // diffuse 
    vec3 light_dir = normalize(light_pos - world_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;
    
    // specular
    float specular_strength = 0.5;
    vec3 view_dir = normalize(camera_pos - world_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;  
        
    vec3 result = (ambient + diffuse + specular) * terrain_color;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(normal.x, normal.y, 0.5f, 1.0f);
};