#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include "shader.hpp"
#include "camera.hpp"
#include "triangle.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;

float delta_time = 0.0f;
float last_frame = 0.0f;

Camera camera(SCR_WIDTH, SCR_HEIGHT);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Reasons", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Utils::Report shader_report;
    Shader basic("src/shaders/triangle.vs", "src/shaders/triangle.fs", shader_report);
    shader_report.log_if_bad();

    Triangle::init();
    std::vector<Triangle> triangles;
    const unsigned triangles_count = 1000;
    for (unsigned i = 0; i < triangles_count; ++i)
    {
        float proportion = (float)i/triangles_count;
        float pos_x = glm::sin(i);
        float pos_y = glm::cos(i);
        float pos_z = -(float)i - 1.0f;
        auto pos_to_push = glm::vec3(pos_x, pos_y, pos_z);

        float rotation_angle = glm::sin(i) * 360.0f;
        auto rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);

        Triangle to_push(pos_to_push, rotation_angle, rotation_axis);
        triangles.push_back(to_push);
    }

    unsigned frame_count_to_show_debug_time = 0;
    while (!glfwWindowShouldClose(window))
    {
        float current_time = glfwGetTime();
        delta_time = current_time - last_frame;
        last_frame = current_time;
        
        if (frame_count_to_show_debug_time > 2*60)
        {
            std::cout<<"Render time: "<<delta_time<<"ms. ("
            << int(1.0f / delta_time) << " FPS)" <<std::endl;
            frame_count_to_show_debug_time = 0;
        }
        ++frame_count_to_show_debug_time;
        

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        basic.use();

        glm::mat4 projection_view = camera.get_projection_view_matrix();
        basic.set_mat4("projection_view", projection_view);
        
        Triangle::bind_vao();
        for (unsigned i = 0; i < triangles_count; ++i)
        {
            basic.set_mat4("model", triangles[i].get_model_matrix());
 
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Triangle::free_opengl_resources();
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(Camera::Direction::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(Camera::Direction::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(Camera::Direction::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(Camera::Direction::RIGHT, delta_time);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (camera.first_mouse)
    {
        camera.last_x = xpos;
        camera.last_y = ypos;
        camera.first_mouse = false;
    }

    float xoffset = xpos - camera.last_x;
    float yoffset = camera.last_y - ypos; 

    camera.last_x = xpos;
    camera.last_y = ypos;

    camera.process_mouse_movement(xoffset, yoffset, delta_time);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.process_mouse_scroll(yoffset, delta_time);
}

