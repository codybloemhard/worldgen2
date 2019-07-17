#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
#include <stdio.h>

#include "window.h"
#include "shader.h"
#include "fps_camera.h"
#include "glstuff.h"

void update(float);
void render(), init(), exit();
void input(GLFWwindow*, float, float, float);

static Buffer<float> *vertices = new Buffer(new float[12]{
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f
}, 12);

static Buffer<int> *indices = new Buffer(new int[6]{
    0, 2, 1,
    2, 3, 1
}, 6);

static GBO<float> *vbo;
static GBO<int> *ebo;
static VAO *vao;
static Shader *shader;
static FpsCamera *cam;

int main(){
    printf("Henlo Frens!\n");
    WindowInit winit = {1600,900,120,60};
    auto window = Window(winit, &init, &update, &render, &input, &exit);
    window.Run();
    window.End();
    printf("Bye!\n");
    return 0;
}

void init(){
    vbo = new GBO<float>(GL_ARRAY_BUFFER);
    vbo->bind();
    vbo->stuff(vertices);
    vbo->upload(GL_STATIC_DRAW);
    ebo = new GBO<int>(GL_ELEMENT_ARRAY_BUFFER);
    ebo->bind();
    ebo->stuff(indices);
    ebo->upload(GL_STATIC_DRAW);
    vao = new VAO();
    vao->bind();
    vao->add_ebo(ebo);
    add_vaa(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0);
    shader = new Shader("shaders/mvp.vs", "shaders/basic.fs");
    cam = new FpsCamera();
    cam->move_sens = 2.0f;
}

void update(float elaps){

}

void render(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();
    cam->apply_mvp(shader);
    vao->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void input(GLFWwindow *window, float elaps, float xpos, float ypos)
{
    cam->input(window, elaps, xpos, ypos);
}

void exit(){
    delete vao;
    delete vbo;
    delete ebo;
}
