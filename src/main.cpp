#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
#include <stdio.h>

#include "window.h"
#include "shader.h"

void update(float);
void render(), init(), exit();
void key_input(GLFWwindow*, float);
void mouse_input(GLFWwindow*, float, float);

static float vertices[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
};
static int indices[] = {
    0, 1, 2
};
static GLuint vao, vbo, ebo;
static Shader* shader;
float pitch, yaw, lastx, lasty;

glm::vec3 campos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 camdir = glm::vec3(0.0f),
    camup = glm::vec3(0.0f),
    camri = glm::vec3(0.0f);

int main(){
    printf("Henlo Frens!\n");
    WindowInit winit = {1600,900,120,60};
    auto window = Window(winit, &init, &update, &render, &key_input, &mouse_input, &exit);
    window.Run();
    window.End();
    printf("Bye!\n");
    return 0;
}

void init(){
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    shader = new Shader("shaders/mvp.vs", "shaders/basic.fs");
}

void update(float elaps){

}

void render(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    camdir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir.y = sin(glm::radians(pitch));
    camdir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir = glm::normalize(camdir);
    camri = glm::normalize(glm::cross(camdir, up));
    camup = glm::normalize(glm::cross(camri, camdir));
    
    glm::mat4 view = glm::lookAt(campos, campos + camdir, camup);
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 16.0f/9.0f, .1f, 100.0f);
    shader->SetMat4("model", glm::mat4(1.0f));
    shader->SetMat4("view", view);
    shader->SetMat4("proj", proj);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void key_input(GLFWwindow *window, float elaps)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        campos += camdir * elaps;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        campos -= camdir * elaps;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        campos -= camri * elaps;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        campos += camri * elaps;
}

void mouse_input(GLFWwindow *window, float xpos, float ypos){
    float dx = xpos - lastx;
    float dy = ypos - lasty;
    lastx = xpos;
    lasty = ypos;
    yaw += dx;
    pitch += dy;
    
    if(pitch > 89.9f)
        pitch = 89.9f;
    if(pitch < -89.9f)
        pitch = -89.9f;
}

void exit(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
