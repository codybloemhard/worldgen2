#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
#include <stdio.h>
#include "../deps/SimplexNoise.h"

#include "window.h"
#include "fps_camera.h"
#include "objects.h"

void update(float);
void render(), init(), exit();
void input(GLFWwindow*, float, float, float);

Terrain *terrain;
static VAO *sea;
static Shader *sea_shader;
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
    terrain = new Terrain();
    {
        sea = new VAO();
        sea->bind();
        auto vertices = new Buffer(new float[12] {
            -1.0f, -1.0f, -1.0f,
            +1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,
        }, 12);
        auto indices = new Buffer(new uint[6] {
            0, 1, 2, 1, 3, 2
        }, 6);
        auto vbo = new GBO<float>(GL_ARRAY_BUFFER);
        vbo->bind();
        vbo->stuff(vertices);
        vbo->upload(GL_STATIC_DRAW);
        auto ebo = new GBO<uint>(GL_ELEMENT_ARRAY_BUFFER);
        ebo->bind();
        ebo->stuff(indices);
        ebo->upload(GL_STATIC_DRAW);
        
        sea->add_ebo(ebo);
        add_vaa(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0);
        sea->unbind();
    }
    sea_shader = new Shader("shaders/sea.vs", "shaders/colour.fs");
    cam = new FpsCamera();
    cam->move_sens = 20.0f;
    cam->fov = 45.0f;
}

void update(float elaps){
    
}

void render(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    terrain->draw(cam);

    sea_shader->Use();
    sea_shader->set_float4("colour", 0.2f, 0.2f, 0.7f, 0.9f);
    sea_shader->set_float("height", terrain->height * 0.2f);
    sea_shader->set_float("size", 1000.0f);
    cam->apply_mvp(sea_shader);
    sea->bind();
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void input(GLFWwindow *window, float elaps, float xpos, float ypos)
{
    cam->input(window, elaps, xpos, ypos);
}

void exit(){
    delete terrain;
    delete sea;
}
