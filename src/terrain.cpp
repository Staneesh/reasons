#include "terrain.hpp"

Terrain::Terrain()
{
    LOG_STRING("Please provide arguments for construction of Terrain struct."); assert(0==1);
}

void Terrain::generate_terrain(
    const glm::vec3& position_pass, 
    float mesh_size_pass, 
    unsigned number_of_tiles_per_side,
    float amplitude_percentage_of_mesh_size,
    unsigned octaves_pass,
    float roughness_pass)
{
    roughness = roughness_pass;
    octaves = octaves_pass;
    amplitude = amplitude_percentage_of_mesh_size * mesh_size_pass;
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

    vertex_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * number_of_vertices);
    triangle_indices = (glm::uvec3*)malloc(sizeof(glm::uvec3) * number_of_triangles);
    normals = (glm::vec3*)malloc(sizeof(glm::vec3) * number_of_vertices);

    unsigned indices_array_index = 0;
    unsigned vertex_array_index = 0; 

    for (unsigned z = 0; z < number_of_tiles_per_side + 1; ++z)
    {
        glm::vec3 current_vertex_position = starting_position + (float)z * dz;
        for (unsigned x = 0; x < number_of_tiles_per_side + 1; ++x)
        {
            current_vertex_position.y = get_height(x, z);

            vertex_positions[vertex_array_index] = current_vertex_position;
            vertex_array_index++;
            current_vertex_position += dx;
            
            unsigned bottom_left = x + z * (number_of_tiles_per_side + 1);
            unsigned top_left = bottom_left + 1;
            unsigned bottom_right = bottom_left + number_of_tiles_per_side + 1;
            unsigned top_right = bottom_right + 1;

            if (x < number_of_tiles_per_side && z < number_of_tiles_per_side)
            {
                triangle_indices[indices_array_index] = 
                glm::uvec3(top_left, bottom_left, bottom_right);
              
                indices_array_index++;

                triangle_indices[indices_array_index] = 
                glm::uvec3(top_left, bottom_right, top_right);
               
                indices_array_index++;
            }
        }
    }

    generate_normals();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(glm::vec3), vertex_positions, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_triangles * sizeof(glm::dvec3) , triangle_indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glGenBuffers(1, &normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void Terrain::generate_normals()
{
    for (unsigned i = 0; i < number_of_vertices; ++i) normals[i] = glm::vec3(0.0f);
    
    for (unsigned i = 0; i < number_of_triangles; ++i)
    {
        unsigned index1 = triangle_indices[i].x;
        unsigned index2 = triangle_indices[i].y;
        unsigned index3 = triangle_indices[i].z;
        glm::vec3 p1 = vertex_positions[index1];
        glm::vec3 p2 = vertex_positions[index2];
        glm::vec3 p3 = vertex_positions[index3];
        glm::vec3 u = p2 - p1, v = p3 - p1;
        
        glm::vec3 current_normal;// = &normals[i];
        current_normal.x = u.y * v.z - u.z * v.y;
        current_normal.y = u.z * v.x - u.x * v.z;
        current_normal.z = u.x * v.y - u.y * v.x;

        current_normal = glm::normalize(current_normal);

        normals[index1] += 0.3f * current_normal;
        normals[index2] += 0.3f * current_normal;
        normals[index3] += 0.3f * current_normal;
    }

    for (unsigned i = 0; i < number_of_vertices; ++i) normals[i] = glm::normalize(normals[i]);
}

void Terrain::draw() const
{
    glBindVertexArray(vao);

    //NOTE(stanisz): wireframe mode for debugging purposes.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, number_of_triangles * 3, GL_UNSIGNED_INT, 0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain::free_opengl_resources()
{
    if (vertex_positions) free(vertex_positions);
    if (triangle_indices) free(triangle_indices);
    if (normals) free(normals);
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);   
}

float Terrain::cosine_interpolation(float a, float b, float blend) const
{
    float theta = blend * M_PI;
    float f = 0.5f * (1.0f - glm::cos(theta));
    return a * (1.0f - f) + b * f;
}

float Terrain::get_interpolated_noise_zero_one(unsigned x, unsigned z) const
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

float Terrain::get_height(unsigned x, unsigned z) const
{
    float result = 0.0f;

    float d = (float)glm::pow(2, octaves - 1);

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
        result += noise * amp;
    }

    result /= (amp_sum);

    result *= amplitude;
    result -= amplitude / 2.0f;
    return result;
}

float Terrain::get_noise_zero_one(unsigned x, unsigned z) const
{
    unsigned seed = x * 49632 + z * 325176;
    srand(seed);
    float res = (float)rand()/(float)RAND_MAX;
    return res;
}

float Terrain::get_smooth_noise_zero_one(unsigned x, unsigned z) const
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

Terrain::Terrain(
    const glm::vec3& position,
    float mesh_size, 
    unsigned number_of_tiles_per_side,
    float amplitude_percentage_of_mesh_size,
    unsigned octaves,
    float roughness
)
{
    generate_terrain(position, mesh_size, number_of_tiles_per_side, 
    amplitude_percentage_of_mesh_size, octaves, roughness);
}