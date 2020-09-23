#ifndef TERRAIN_H
#define TERRAIN_H

#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <random>

class Terrain
{
    unsigned number_of_vertices;
    unsigned number_of_triangles;
    std::vector<glm::vec3> vertex_positions;
    std::vector<glm::uvec3 > triangle_indices; 

    unsigned vbo, vao, ebo;
    float mesh_size;
    float tile_size;
    glm::vec3 position;

    float amplitude;
    unsigned octaves;

    float cosine_interpolation(float a, float b, float blend);
    float get_noise_zero_one(unsigned x, unsigned z);
    float get_smooth_noise_zero_one(unsigned x, unsigned z);
    float get_interpolated_noise_zero_one(unsigned x, unsigned z);
    float get_height(unsigned x, unsigned z);

public:

    void generate(
        const glm::vec3& position,
        float mesh_size, 
        unsigned number_of_tiles_per_side,
        float amplitude,
        unsigned octaves);

    void draw();

    void free_opengl_resources();
};

#endif