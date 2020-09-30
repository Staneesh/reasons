#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include "shader.hpp"
#include "camera.hpp"
#include "triangle.hpp"
#include "terrain.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;

float delta_time = 0.0f;
float last_frame = 0.0f;

Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.5f, 0.0f));

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
    Shader terrain_shader("src/shaders/terrain.vs", "src/shaders/terrain.fs", shader_report);
    shader_report.log_if_bad();

    Terrain terrain(glm::vec3(0.0f), 5.0f, 300, 0.3f, 3, 0.3f);

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

        glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection_view = camera.get_projection_view_matrix();

        terrain_shader.use();
        terrain_shader.set_mat4("projection_view", projection_view);
        terrain.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terrain.free_opengl_resources();

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
    UNUSED(window);

    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    UNUSED(window);

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
    UNUSED(window);
    UNUSED(xoffset);

    camera.process_mouse_scroll(yoffset, delta_time);
}

