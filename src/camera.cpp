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

Camera::Camera(unsigned int screen_width_pass, unsigned int screen_height_pass,
    const vec3& position_pass, const vec3& up_pass, float yaw_pass, float pitch_pass)
{
    position = position_pass;
    up = up_pass;
    yaw = yaw_pass;
    pitch = pitch_pass;
    sensitivity = default_sensitivity;
    zoom = default_zoom;
    movement_speed = default_speed;
    world_up = up;

    front = vec3(0.0f, 0.0, -1.0f);

    first_mouse = true;
    
    screen_height = screen_height_pass;
    screen_width = screen_width_pass;

    last_x = screen_width / 2.0f;
    last_y = screen_width / 2.0f;

    update_vectors();
    //std::cout<<up.x<<' '<<up.y<<' '<<up.z<<std::endl;

}

glm::mat4 Camera::get_view_matrix()
{
    return lookAt(position, position + front, up);
}

glm::mat4 Camera::get_projection_matrix()
{
    return perspective(radians(zoom), (float)screen_width / (float)screen_height, 
    0.1f, 100.0f);
}

void Camera::process_keyboard(const Direction& dir, float delta_time)
{
    float speed = movement_speed * delta_time;

    vec3 velocity = glm::vec3(0.0f);

    if (dir == Direction::FORWARD)
    {
        velocity += front;
    }
    if (dir == Direction::BACKWARD)
    {
        velocity -= front;
    }
    if (dir == Direction::LEFT)
    {
        velocity -= right;
    }
    if (dir == Direction::RIGHT)
    {
        velocity += right;
    }

    velocity = normalize(velocity);
    velocity *= speed;
    
    position += velocity;
}

void Camera::process_mouse_movement(float x_offset, float y_offset, float delta_time)
{
    x_offset *= sensitivity * delta_time * 50.0f;
    y_offset *= sensitivity * delta_time * 50.0f;
    
    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.00f)
    {
        pitch = 89.00f;
    }
    if (pitch < -89.00f)
    {
        pitch = -89.00f;
    }

    update_vectors();
}

void Camera::process_mouse_scroll(float y_offset, float delta_time)
{
    zoom -= y_offset * delta_time * 50.0f;
    if (zoom < 1.0f)
    {
        zoom = 1.0f;
    }
    if (zoom > 45.0f)
    {
        zoom = 45.0f;
    }
}