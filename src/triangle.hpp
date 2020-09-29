#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Triangle
{
    static const float vertices[9];
    static unsigned vbo, vao;
    
public:

    glm::vec3 position;
    float rotation_angle;
    glm::vec3 rotation_axis;
    glm::vec3 scaling_vector;

    Triangle(
        const glm::vec3& position, float rotation_angle = 0.0f, 
        const glm::vec3& rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f), 
        const glm::vec3& scaling_vector = glm::vec3(1.0f));
        
    static void init();    
    static void free_opengl_resources();
    static void bind_vao();
    
    glm::mat4 get_model_matrix() const;
};

#endif