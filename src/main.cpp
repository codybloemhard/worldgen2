#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
#include <stdio.h>
#include "../deps/SimplexNoise.h"

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

static Buffer<uint> *indices = new Buffer(new uint[6]{
    0, 2, 1,
    2, 3, 1
}, 6);

static GBO<float> *vbo;
static GBO<uint> *ebo;
static VAO *vao;
uint size = 640;
float scale = 0.002f, height = 0.0f;
SimplexNoise *noise = new SimplexNoise();
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
    uint vert_len = (size+1)*(size+1);
    vertices = new Buffer(new float[vert_len * 3], vert_len * 3);
    for(int i = 0; i < vert_len; i++){
        uint x = (i % (size+1));
        uint y = (i / (size+1));
        vertices->data[i * 3 + 0] = x;
        float h = noise->fractal(5, x * scale, y * scale);
        vertices->data[i * 3 + 1] = h*0.5f+0.5f;
        vertices->data[i * 3 + 2] = y;
    }
    uint indi_len = size * size * 6;
    indices = new Buffer(new uint[indi_len], indi_len);
    int i = 0;
    for(int x = 0; x < size; x++)for(int y = 0; y < size; y++){
        indices->data[i + 0] = (x + 0) + (y + 0) * (size + 1);
        indices->data[i + 1] = (x + 1) + (y + 0) * (size + 1);
        indices->data[i + 2] = (x + 1) + (y + 1) * (size + 1);
        indices->data[i + 3] = (x + 0) + (y + 0) * (size + 1);
        indices->data[i + 4] = (x + 1) + (y + 1) * (size + 1);
        indices->data[i + 5] = (x + 0) + (y + 1) * (size + 1);
        i += 6;
    }

    vbo = new GBO<float>(GL_ARRAY_BUFFER);
    vbo->bind();
    vbo->stuff(vertices);
    vbo->upload(GL_STATIC_DRAW);
    ebo = new GBO<uint>(GL_ELEMENT_ARRAY_BUFFER);
    ebo->bind();
    ebo->stuff(indices);
    ebo->upload(GL_STATIC_DRAW);
    vao = new VAO();
    vao->bind();
    vao->add_ebo(ebo);
    add_vaa(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0);
    shader = new Shader("shaders/terrain.vs", "shaders/terrain.fs");
    cam = new FpsCamera();
    cam->move_sens = 20.0f;
}

void update(float elaps){
    shader->Use();
    height = 64.0f;
    height += elaps * 4;
    if(height > 64)
        height = 64.0f;
    shader->set_float("height", height);
}

void render(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();
    cam->apply_mvp(shader);
    vao->bind();
    glDrawElements(GL_TRIANGLES, size*size*6, GL_UNSIGNED_INT, 0);
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
