#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
#include <stdio.h>

#include "window.h"
#include "shader.h"

void update(double);
void render(), init(), exit();

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
float pitch, yaw;

int main(){
    printf("Henlo Frens!\n");
    WindowInit winit = {1600,900,120,60};
    auto window = Window(winit, &init, &update, &render, &exit);
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

void update(double elaps){
    pitch = 0.0f;//+= elaps * 20.0f;
    yaw += elaps * 50.0f;
    if(pitch > 89.9f)
        pitch = 89.9f;
    if(pitch < -89.9f)
        pitch = -89.9f;
}

void render(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();
    glm::vec3 campos = glm::vec3(0.0f, 0.0f, -5.0f);
    glm::vec3 camdir = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    camdir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir.y = sin(glm::radians(pitch));
    camdir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir = glm::normalize(camdir);
    glm::vec3 camri = glm::normalize(glm::cross(camdir, up));
    glm::vec3 camup = glm::normalize(glm::cross(camri, camdir));
    
    glm::mat4 view = glm::lookAt(campos, campos + camdir, camup);
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 16.0f/9.0f, .1f, 100.0f);
    shader->SetMat4("model", glm::mat4(1.0f));
    shader->SetMat4("view", view);
    shader->SetMat4("proj", proj);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void exit(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
