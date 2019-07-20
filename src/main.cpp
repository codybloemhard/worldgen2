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
Sea *sea;
Sky *sky;
FpsCamera *cam;

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
    sea = new Sea(terrain->height * 0.2f);
    sky = new Sky();
    cam = new FpsCamera();
    cam->move_sens = 20.0f;
    cam->fov = 45.0f;
    cam->far = 5000.0f;
}

void update(float elaps){
    
}

void render(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    terrain->draw(cam);
    sea->draw(cam);
    sky->draw(cam);

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
