#include "terrain.hpp"

Terrain::Terrain()
{
    LOG_STRING("Please provide arguments for construction of Terrain struct."); assert(0==1);
}

void Terrain::generate_terrain(
    const glm::vec3& position_pass, 
    float mesh_size_pass, 
    unsigned number_of_tiles_per_side,
    float amplitude_pass,
    unsigned octaves_pass,
    float roughness_pass)
{   
    BEGIN_TIMED_BLOCK("Terrain generation");

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

    vertex_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * number_of_vertices);
    triangle_indices = (glm::uvec3*)malloc(sizeof(glm::uvec3) * number_of_triangles);
    normals = (glm::vec3*)malloc(sizeof(glm::vec3) * number_of_vertices);

    unsigned indices_array_index = 0;
    unsigned vertex_array_index = 0; 

    for (unsigned z = 0; z < number_of_tiles_per_side + 1; ++z)
    {
#if 0
        float terrain_generation_percentage = 100.0f * z/number_of_tiles_per_side;
        LOG(terrain_generation_percentage);
#endif
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

#if 0
    u32 elems = 0;
    glm::uvec3* a;
    for(a = triangle_indices; elems < number_of_triangles; ++a)
    {
        ++elems;
    }
    LOG(elems);
    LOG(a - triangle_indices);
    LOG(number_of_triangles);
#endif

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
#if 1
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(glm::vec3), vertex_positions, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_triangles * sizeof(glm::uvec3) , triangle_indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glGenBuffers(1, &normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
#else
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    u32 pos_size = number_of_vertices * sizeof(glm::vec3), tri_size =  number_of_triangles * sizeof(glm::dvec3);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, pos_size, &vertex_positions);
    glBufferSubData(GL_ARRAY_BUFFER, pos_size, pos_size, &normals);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_size, &triangle_indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::dvec3), (void*)(pos_size));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
#endif
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

float Terrain::cosine_interpolation(float a, float b, float blend) 
{
    float theta = blend * M_PI;
    float f = 0.5f * (1.0f - glm::cos(theta));
    //return (1.0f - blend) * a + blend * b;
    return a * (1.0f - f) + b * f;
}

float Terrain::get_interpolated_noise_zero_one(float x, float z) 
{
    int int_x = (int)x;
    int int_z = (int)z;
    float fract_x = x - (float)int_x;
    float fract_z = z - (float)int_z;

    float v1 = get_smooth_noise_zero_one(int_x, int_z);
    float v2 = get_smooth_noise_zero_one(int_x + 1, int_z);
    float v3 = get_smooth_noise_zero_one(int_x, int_z + 1);
    float v4 = get_smooth_noise_zero_one(int_x + 1, int_z + 1);

    float i1 = cosine_interpolation(v1, v2, fract_x);
    float i2 = cosine_interpolation(v3, v4, fract_x);

    return cosine_interpolation(i1, i2, fract_z);
}

float Terrain::get_height(float x, float z) 
{
    float result = 0.0f;

    float d = (float)(1<<(octaves-1));

    float amp_sum = 0.0f;
    for (unsigned i = 0; i < octaves; ++i)
    {
        amp_sum += (float)glm::pow(roughness, i); 
    }

    float offset = 0.0f;
    for (unsigned i = 0; i < octaves; ++i)
    {
        float frequency = (float)(1<<i) / d;
        float amp = (float)glm::pow(roughness, i) * amplitude;

        
        float noise = get_interpolated_noise_zero_one(frequency * (x + offset), frequency * (z + offset)) * 2.0f - 1.0f;
        result += noise * amp;
    }

    //result = get_interpolated_noise_zero_one((float)(x + offset) / 8, (float)(z + offset) / 8) * 2.0f - 1.0f;
    //result *= amplitude;
    //result /= (amp_sum);

    return result;
}


#if 0
/* The state array must be initialized to not be all zero */
unsigned xorshift128()
{
	/* Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs" */
	unsigned t = xorshift_state.d;

	unsigned const s = xorshift_state.a;
	xorshift_state.d = xorshift_state.c;
	xorshift_state.c = xorshift_state.b;
	xorshift_state.b = s;

	t ^= t << 11;
	t ^= t >> 8;
    xorshift_state.a = t ^ s ^ (s >> 19);
   
	return xorshift_state.a;
}
#endif

unsigned Terrain::xorshift()
{
    uint64_t x = xorshift_state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return xorshift_state = x;
}


float Terrain::get_noise_zero_one(float x, float z)
{
    unsigned cur_seed = (unsigned)(((unsigned long long)terrain_seed + (unsigned)x * 49632 + (unsigned)z * 325176) % 1000007);
    xorshift_state = cur_seed;
   
    float res = (float)xorshift()/(float)(UINT32_MAX); 
    return res;
}

float Terrain::get_smooth_noise_zero_one(float x, float z) 
{
    float sides = 
    get_noise_zero_one(x + 1, z) + 
    get_noise_zero_one(x - 1, z) + 
    get_noise_zero_one(x, z + 1) +
    get_noise_zero_one(x, z - 1);

    sides /= 8.0f;
    
    float corners = 
    get_noise_zero_one(x + 1, z + 1) +
    get_noise_zero_one(x - 1, z - 1) +
    get_noise_zero_one(x - 1, z + 1) +
    get_noise_zero_one(x + 1, z - 1);

    corners /= 16.0f;

    float center = get_noise_zero_one(x, z);

    center /= 4.0f;

    float res = (sides + corners + center);
    return res;
}

Terrain::Terrain(
    const glm::vec3& position_pass,
    float mesh_size_pass, 
    unsigned number_of_tiles_per_side_pass,
    float amplitude_pass,
    unsigned octaves_pass,
    float roughness_pass
)
{
    terrain_seed = 1000;
    xorshift_state = terrain_seed;

    generate_terrain(position_pass, mesh_size_pass, number_of_tiles_per_side_pass, 
    amplitude_pass, octaves_pass, roughness_pass);
}