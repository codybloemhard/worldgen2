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
    shader = new Shader("shaders/basic.vs", "shaders/basic.fs");
}

void update(double elaps){

}

void render(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void exit(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
