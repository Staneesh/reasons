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

    static void init();
    Triangle(const glm::vec3& position);
    glm::mat4 get_model_matrix() const;
    static void free_opengl_resources();
    static void bind_vao();
};

#endif