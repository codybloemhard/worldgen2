#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
#include <stdio.h>
#include "../deps/SimplexNoise.h"

#include "window.h"
#include "fps_camera.h"
#include "objects.h"
#include "font.h"
#include "worldstate.h"
#include "mathh.h"

void update(float);
void render(), init(), exit();
void input(GLFWwindow*, float, float, float);

uint ww = 1600, wh = 900;
//uint ww = 2560, wh = 1440;

Terrain *terrain;
Sea *sea;
Sky *sky;
FpsCamera *cam;
Font* font;

GLuint fbo, tex;
float gametime = 0.0f;

bool show_debug = true;
float font_size = 0.02f;
glm::vec3 last_pos = glm::vec3(0);
float total_dist_r3 = 0;
float total_dist_xz = 0;

uint mode = 0;
bool m_down = false;

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
    WorldState::Get().sun_dir = glm::normalize(glm::vec3(0.5f, -1.0f, 0.5f));
    WorldState::Get().sea_level = 0.25f;
    WorldState::Get().world_height = 3000.0f;
    WorldState::Get().world_scale = 0.0001f;
    WorldState::Get().walk_speed = 5.0f;
    WorldState::Get().fly_speed = 500.0f;
    font = new Font("assets/codyfont.png", 12, 8, 16.0f/9.0f);
    terrain = new Terrain();
    sea = new Sea();
    sky = new Sky();
    cam = new FpsCamera();
    cam->fov = 45.0f;
    cam->far = 1000000.0f;

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
    total_dist_r3 += glm::distance(last_pos, cam->campos);
    total_dist_xz += glm::distance(glm::vec3(last_pos.x,cam->campos.y,last_pos.z), cam->campos);
    last_pos = cam->campos;
    if(mode == 0){//fly mode
        cam->move_sens = WorldState::Get().fly_speed;
        cam->vert_sens = WorldState::Get().fly_speed;
    }else if(mode == 1){//walk mode
        cam->move_sens = WorldState::Get().walk_speed;
        cam->vert_sens = 0.0f;
        float h = Mathh::terrain_noise(cam->campos.x, cam->campos.z);
        cam->campos.y = h + 2.0f;
    }
}

void render(){
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    terrain->draw(cam);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    terrain->dep_draw(cam);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    sea->draw(cam, tex, ww, wh, gametime);
    sky->draw(cam);
    if(show_debug){
        uint end = font->print_grid(std::string("X: "), (int)cam->campos.x, font_size, glm::vec3(0.2f), 0, 0);
        end = font->print_grid(std::string("Y: "), (int)cam->campos.y, font_size, glm::vec3(0.2f), end + 1, 0);
        end = font->print_grid(std::string("Z: "), (int)cam->campos.z, font_size, glm::vec3(0.2f), end + 1, 0);
        font->print_grid(std::string("R3-DIST-ORI: "), (uint)glm::distance(glm::vec3(0.0f), cam->campos), font_size, glm::vec3(0.2f), 0, 1);
        font->print_grid(std::string("R3-DIST-TOT: "), (uint)total_dist_r3, font_size, glm::vec3(0.2f), 0, 2);
        font->print_grid(std::string("XZ-DIST-ORI: "), (uint)glm::distance(glm::vec3(0,cam->campos.y,0), cam->campos), font_size, glm::vec3(0.2f), 0, 3);
        font->print_grid(std::string("XZ-DIST-TOT: "), (uint)total_dist_xz, font_size, glm::vec3(0.2f), 0, 4);
    }
    glBindVertexArray(0);
}

void input(GLFWwindow *window, float elaps, float xpos, float ypos)
{
    bool m_down_prev = m_down;
    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        m_down = true;
    else m_down = false;
    if(m_down_prev != m_down && m_down)
        mode = (mode + 1) % 2;
    cam->input(window, elaps, xpos, ypos);
}

void exit(){
    delete terrain;
    delete sea;
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &tex);
}
