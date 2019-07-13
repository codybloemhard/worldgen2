#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
#include <stdio.h>

#include "shader.h"

void error_callback(int, const char*);
static void key_callback(GLFWwindow*, int, int, int, int);

static struct Globals{
    int ww = 1600, wh = 900;
    int fbw = 0, fbh = 0;
} globals;

int main(){
    printf("Henlo Frens\n");

    if(!glfwInit()){ // init glfw
        printf("Error: could not init glfw.\n");
        return -1;
    }
    // what function to handle errors
    glfwSetErrorCallback(error_callback);
    // prefered and minimum openl gl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // create window
    GLFWwindow* window = glfwCreateWindow(globals.ww, globals.wh, "Henlo Frens", NULL, NULL);
    if(window == NULL){
        printf("Error: could not create window.\n");
        glfwTerminate();
        return -1;
    }
    // use this window to capture input
    glfwSetKeyCallback(window, key_callback);
    // use opengl with this window
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(0);//1 for vsync
    glfwGetFramebufferSize(window, &globals.fbw, &globals.fbh);
    glViewport(0, 0, globals.fbw, globals.fbh);
    
    //example
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    auto shader = Shader("shaders/basic.vs", "shaders/basic.fs");

    // game loop
    double last_checkpoint = glfwGetTime();
    int update_ticks = 0;
    int render_ticks = 0;
    while(!glfwWindowShouldClose(window)){
        auto now = glfwGetTime();
        if(now - last_checkpoint > 1.0){
            last_checkpoint = now;
            printf("Ticks: %d, Frames: %d\n", update_ticks, render_ticks);
            update_ticks = 0;
            render_ticks = 0;
        }
        if((now - last_checkpoint) * 120 > update_ticks){
            glfwPollEvents();
            update_ticks++;
        }
        if((now - last_checkpoint) * 60 < render_ticks)
            continue;
        render_ticks++;
        //render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
    }
    // end program
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Bye!\n");
    return 0;
}

void error_callback(int error, const char* description){
    printf("Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
