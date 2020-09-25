#ifndef TERRAIN_H
#define TERRAIN_H

#include "../include/glad/glad.h"
#include "utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <random>

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

    unsigned vbo, vao, ebo, normal_vbo;
    float mesh_size;
    float tile_size;
    glm::vec3 position;

    float amplitude;
    unsigned octaves;
    float roughness;

    float cosine_interpolation(float a, float b, float blend) const;
    float get_noise_zero_one(unsigned x, unsigned z) const;
    float get_smooth_noise_zero_one(unsigned x, unsigned z) const;
    float get_interpolated_noise_zero_one(unsigned x, unsigned z) const;
    float get_height(unsigned x, unsigned z) const;

    void generate_normals();
public:

    void generate(
        const glm::vec3& position,
        float mesh_size, 
        unsigned number_of_tiles_per_side,
        float amplitude_percentage_of_mesh_size,
        unsigned octaves,
        float roughness);

    void draw() const;

    void free_opengl_resources();
};

#endif