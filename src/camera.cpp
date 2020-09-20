#include "camera.hpp"

void Camera::update_vectors()
{
    vec3 new_front;
    new_front.x = cos(radians(yaw)) * cos(radians(pitch));
    new_front.y = sin(radians(pitch));
    new_front.z = sin(radians(yaw)) * cos(radians(pitch));

    front = normalize(new_front);
    right = normalize(cross(front, world_up));
    up = normalize(cross(right, front));
}

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
{

}