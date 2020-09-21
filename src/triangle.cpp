#include "triangle.hpp"

unsigned Triangle::vao = 0;
unsigned Triangle::vbo = 0;
const float Triangle::vertices[] = {
        -0.5f, -0.5f, 0.0f,  
         0.5f, -0.5f, 0.0f, 
         0.0f,  0.5f, 0.0f   
    }; 


void Triangle::init()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    bind_vao();
}

Triangle::Triangle(const glm::vec3& position_pass, float rotation_angle_pass, 
        const glm::vec3& rotation_axis_pass, const glm::vec3& scaling_vector_pass)
{
    position = position_pass;
    rotation_angle = rotation_angle_pass;
    rotation_axis = rotation_axis_pass;
    scaling_vector = scaling_vector_pass;
}

glm::mat4 Triangle::get_model_matrix() const
{
    glm::mat4 result(1.0f);

    result = glm::translate(result, position);
    result = glm::rotate(result, glm::radians(rotation_angle), rotation_axis);
    result = glm::scale(result, scaling_vector);

    return result;
}

void Triangle::free_opengl_resources()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Triangle::bind_vao()
{
    glBindVertexArray(vao);
}