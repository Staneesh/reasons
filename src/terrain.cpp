#include "terrain.hpp"

void Terrain::generate(
    const glm::vec3& position_pass, 
    float mesh_size_pass, 
    unsigned number_of_tiles_per_side,
    float amplitude_pass,
    unsigned octaves_pass,
    float roughness_pass)
{
    roughness = roughness_pass;
    octaves = octaves_pass;
    amplitude = amplitude_pass;
    number_of_triangles = number_of_tiles_per_side * number_of_tiles_per_side * 2;
    position = position_pass;
    mesh_size = mesh_size_pass;
    tile_size = mesh_size_pass / number_of_tiles_per_side;
    number_of_vertices = (number_of_tiles_per_side + 1) * (number_of_tiles_per_side + 1);

    glm::vec3 starting_position = glm::vec3(
        position.x - mesh_size / 2,
        position.y, 
        position.z - mesh_size / 2);

    glm::vec3 dz = glm::vec3(0.0f, 0.0f, tile_size);
    glm::vec3 dx = glm::vec3(tile_size, 0.0f, 0.0f);

    //vertex_positions = new std::vector<glm::fvec3>(number_of_vertices);
    //triangle_indices = new std::vector<glm::uvec3>(number_of_triangles);
    vertex_positions.reserve(number_of_triangles);
    triangle_indices.reserve(number_of_triangles);

    //std::cout<<"S:"<<vertex_positions->size()<<std::endl;

    for (unsigned z = 0; z < number_of_tiles_per_side + 1; ++z)
    {
        glm::vec3 current_vertex_position = starting_position + (float)z * dz;
        for (unsigned x = 0; x < number_of_tiles_per_side + 1; ++x)
        {
            current_vertex_position.y = get_height(x, z);
            vertex_positions.push_back(current_vertex_position);
            //std::cout<<current_vertex_position.x<<" "<<current_vertex_position.y<<" "<<current_vertex_position.z<<std::endl;
            current_vertex_position += dx;
            
            unsigned bottom_left = x + z * (number_of_tiles_per_side + 1);
            unsigned top_left = bottom_left + 1;
            unsigned bottom_right = bottom_left + number_of_tiles_per_side + 1;
            unsigned top_right = bottom_right + 1;

            if (x < number_of_tiles_per_side && z < number_of_tiles_per_side)
            {
                triangle_indices.push_back(glm::uvec3(top_left, bottom_left, bottom_right));
                triangle_indices.push_back(glm::uvec3(top_left, bottom_right, top_right));
            }
        }
    }

#if 0
    std::cout<<"INDICES:"<<std::endl;
    for (unsigned i = 0; i < triangle_indices.size(); ++i)
    {
        auto cur = triangle_indices[i];
        std::cout<<cur.x<<' '<<cur.y<<' '<<cur.z<<std::endl;
    }
#endif 

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(glm::vec3), &vertex_positions[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_indices.size() * sizeof(glm::dvec3), &triangle_indices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void Terrain::draw()
{
    glBindVertexArray(vao);
    //NOTE(stanisz): wireframe mode for debugging purposes.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, number_of_triangles * 3, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain::free_opengl_resources()
{
    //delete vertex_positions;
    //delete triangle_indices;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);   
}

float Terrain::cosine_interpolation(float a, float b, float blend)
{
    float theta = blend * M_PI;
    float f = 0.5f * (1.0f - glm::cos(theta));
    return a * (1.0f - f) + b * f;
}

float Terrain::get_interpolated_noise_zero_one(unsigned x, unsigned z)
{
    int int_x = (int)x;
    int int_z = (int)z;
    float fract_x = x - int_x;
    float fract_z = z - int_z;

    float v1 = get_smooth_noise_zero_one(int_x, int_z);
    float v2 = get_smooth_noise_zero_one(int_x + 1, int_z);
    float v3 = get_smooth_noise_zero_one(int_x, int_z + 1);
    float v4 = get_smooth_noise_zero_one(int_x + 1, int_z + 1);

    float i1 = cosine_interpolation(v1, v2, fract_x);
    float i2 = cosine_interpolation(v3, v4, fract_x);

    return cosine_interpolation(i1, i2,fract_z);
}

float Terrain::get_height(unsigned x, unsigned z)
{
    float result = 0.0f;

    float d = (float)glm::pow(2, octaves - 1);

    //std::cout<<"O:"<<octaves_power<<" "<<octaves_power_sum<<std::endl;
    
    float amp_sum = 0.0f;
    for (unsigned i = 0; i < octaves; ++i)
    {
        amp_sum += (float)glm::pow(roughness, i); 
    }

    for (unsigned i = 0; i < octaves; ++i)
    {
        float frequency = (float)glm::pow(2, i) / d;
        float amp = (float)glm::pow(roughness, i);

        float noise = get_interpolated_noise_zero_one(frequency * x, frequency * z);
        //std::cout<<"noise:"<<noise<<std::endl;
        result += noise * amp;
    }

    result /= (amp_sum);
    //std::cout<<"result:"<<result<<std::endl;

    result *= amplitude;
    result -= amplitude / 2.0f;

    //std::cout<<"result:"<<result<<std::endl;
    return result;
}

float Terrain::get_noise_zero_one(unsigned x, unsigned z)
{
    unsigned seed = x * 49632 + z * 325176;
    srand(seed);
    float res = (float)rand()/(float)RAND_MAX;
    return res;
}

float Terrain::get_smooth_noise_zero_one(unsigned x, unsigned z)
{
    float sides = 
    get_noise_zero_one(x + 1, z) + 
    get_noise_zero_one(x - 1, z) + 
    get_noise_zero_one(x, z + 1) +
    get_noise_zero_one(x, z - 1);
    
    float corners = 
    get_noise_zero_one(x + 1, z + 1) +
    get_noise_zero_one(x - 1, z - 1) +
    get_noise_zero_one(x - 1, z + 1) +
    get_noise_zero_one(x + 1, z - 1);

    float center = get_noise_zero_one(x, z);

    float res = (sides + corners + center) / 9.0f;
    return res;
}