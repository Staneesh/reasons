#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

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

public:

    void generate(
        const glm::vec3& position,
        float mesh_size, 
        unsigned number_of_tiles_per_side);

};

#endif