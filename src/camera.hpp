#ifndef CAMERA_H
#define CAMERA_H

#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

class Camera
{
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;

    float yaw, pitch;
    float movement_speed;
    float sensitivity;
    float zoom;

    //NOTE(stanisz): position on the screen.
    
    float screen_width, screen_height;
    

    static const float constexpr default_yaw = -90.0f;
    static const float constexpr default_pitch = 0.0f;
    static const float constexpr default_speed = 2.5f;
    static const float constexpr default_sensitivity = 0.1f;
    static const float constexpr default_zoom = 45.0f;
    
    void update_vectors();
public:
    float last_x, last_y;
    bool first_mouse;

    enum class Direction
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(unsigned int screen_width, unsigned int screen_height,
        const vec3& position = vec3(0.0f, 0.0f, 0.0f), 
        const vec3& up = vec3(0.0f, 1.0f, 0.0f), 
        float yaw = default_yaw, 
        float pitch = default_pitch);

    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();

    void process_keyboard(const Direction& dir, float delta_time);
    //TODO(stanisz): delta_time needed in both of theese?
    void process_mouse_movement(float x_offset, float y_offset, float delta_time);
    void process_mouse_scroll(float y_offset, float delta_time);
};

#endif