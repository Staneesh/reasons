#include "terrain.hpp"

void Terrain::generate(
    const glm::vec3& position_pass, 
    float mesh_size_pass, 
    unsigned number_of_tiles_per_side)
{
    number_of_triangles = number_of_tiles_per_side * number_of_tiles_per_side * 2;
    position = position_pass;
    mesh_size = mesh_size_pass;
    tile_size = mesh_size_pass / number_of_tiles_per_side;
    number_of_vertices = (number_of_tiles_per_side + 1) * (number_of_tiles_per_side + 1);

    glm::vec3 starting_position = glm::vec3(
        position.x - mesh_size / 2,
        position.y, 
        position.z - mesh_size / 2);

    glm::vec3 dz = glm::vec3(0.0f, 0.0f, -tile_size);
    glm::vec3 dx = glm::vec3(tile_size, 0.0f, 0.0f);

    for (unsigned z = 0; z < number_of_tiles_per_side + 1; ++z)
    {
        glm::vec3 current_vertex_position = starting_position + (float)z * dz;
        for (unsigned x = 0; x < number_of_tiles_per_side + 1; ++x)
        {
            vertex_positions.push_back(current_vertex_position);
            //std::cout<<current_vertex_position.x<<" "<<current_vertex_position.y<<" "<<current_vertex_position.z<<std::endl;
            current_vertex_position += dx;
            
            unsigned bottom_left = x + z * (number_of_tiles_per_side + 1);
            unsigned bottom_right = bottom_left + 1;
            unsigned top_left = bottom_left + number_of_tiles_per_side + 1;
            unsigned top_right = top_left + 1;
            triangle_indices.push_back(glm::uvec3(x + 1, x, number_of_tiles_per_side + 1 + x));
            triangle_indices.push_back(glm::uvec3(x + 1, number_of_tiles_per_side + 1 + x, number_of_tiles_per_side + 2 + x));
        }
    }
    
#if 0
    for (unsigned i = 0; i < triangle_indices.size(); ++i)
    {
        auto cur = triangle_indices[i];
        std::cout<<cur.x<<' '<<cur.y<<' '<<cur.z<<std::endl;
    }
#endif 

}