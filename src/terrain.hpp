#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Terrain
{
    unsigned number_of_vertices;
    std::vector<glm::vec3> vertex_positions;
    unsigned vbo, vao;
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