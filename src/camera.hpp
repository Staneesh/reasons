#ifndef CAMERA_H
#define CAMERA_H

#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    static const float constexpr default_yaw = -90.0f;
    static const float constexpr default_pitch = 0.0f;
    static const float constexpr default_speed = 2.5f;
    static const float constexpr default_sensitivity = 0.1f;
    static const float constexpr default_zoom = 45.0f;
    
    void update_vectors();
public:

    Camera(
        vec3 position = vec3(0.0f, 0.0f, 0.0f), 
        vec3 up = vec3(0.0f, 1.0f, 0.0f), 
        float yaw = default_yaw, 
        float pitch = default_pitch);

    
};

#endif