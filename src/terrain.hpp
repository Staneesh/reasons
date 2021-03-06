#ifndef TERRAIN_H
#define TERRAIN_H

#include "../include/glad/glad.h"
#include "utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Terrain
{
    unsigned number_of_vertices;
    unsigned number_of_triangles;
    glm::vec3* vertex_positions;
    glm::uvec3* triangle_indices; 
    //NOTE(stanisz): the size of this structure should be equal to
    // the 'number_of_vertices' - each vertex has its normal
    // which is equal to the arithmetic mean of the normals of
    // adjacent faces.
    glm::vec3* normals;

    unsigned vbo, vao, ebo;
    //TODO(stanisz): replace glBufferData with glBufferSubdata in order to use only 1 vbo.
    unsigned normal_vbo; 
    float mesh_size;
    float tile_size;
    //NOTE(stanisz): position of the center of the terrain mesh in world coords.
    glm::vec3 position;

    float amplitude;
    unsigned octaves;
    float roughness;

    u32 xorshift_state;
    unsigned terrain_seed;

    float cosine_interpolation(float a, float b, float blend);
    float get_noise_zero_one(float x, float z);
    float get_smooth_noise_zero_one(float x, float z);
    float get_interpolated_noise_zero_one(float x, float z);
    float get_height(float x, float z);

    void generate_normals();

    //NOTE(stanisz): a strip of land
    struct TerrainGenerationJobData
    {
        unsigned z_start, z_end;
        unsigned tiles_per_side;
    };
    void terrain_generation_job(const TerrainGenerationJobData& d, 
        glm::vec3* vertex_positions);
    void generate_terrain(
        const glm::vec3& position,
        float mesh_size, 
        unsigned number_of_tiles_per_side,
        float amplitude_pass,
        unsigned octaves,
        float roughness);

public:

    Terrain(
        const glm::vec3& position,
        float mesh_size, 
        unsigned number_of_tiles_per_side,
        float amplitude_percentage_of_mesh_size,
        unsigned octaves,
        float roughness);

    Terrain();
    void draw() const;
    void free_opengl_resources();
};

#endif