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

//uint ww = 1600, wh = 900;
uint ww = 2560, wh = 1440;

Terrain *terrain;
Sea *sea;
Sky *sky;
FpsCamera *cam;

GLuint fbo, tex;
float gametime = 0.0f;

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
    WorldState::Get().sun_dir = glm::normalize(glm::vec3(0.5f, -1.0f, 0.5f));
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glDeleteTextures(1, &tex);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ww, wh, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    glBindTexture(GL_TEXTURE, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ww, wh);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        printf("Error: could not set up fbo\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void update(float elaps){
    gametime += elaps;
}

void render(){
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    terrain->draw(cam);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    terrain->dep_draw(cam);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    sea->draw(cam, tex, ww, wh, gametime);
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
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &tex);
}
