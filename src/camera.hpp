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

    float screen_width, screen_height;
    
    static const float constexpr default_yaw = -90.0f;
    static const float constexpr default_pitch = 0.0f;
    static const float constexpr default_speed = 2.5f;
    static const float constexpr default_sensitivity = 0.1f;
    static const float constexpr default_zoom = 45.0f;
    
    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_matrix() const;

    void update_vectors();

public:
    //NOTE(stanisz): position of mouse on the screen 
    // during last frame.
    float last_x, last_y;
    //NOTE(stanisz): if true, the mouse has never been moved. 
    // False otherwise.
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
        float pitch = default_pitch,
        float sensitivity = default_sensitivity,
        float speed = default_speed
        );

    glm::mat4 get_projection_view_matrix() const;

    void process_keyboard(const Direction& dir, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset, float delta_time);
    void process_mouse_scroll(float y_offset, float delta_time);
};

#endif