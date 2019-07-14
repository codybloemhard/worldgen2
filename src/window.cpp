#include "window.h"

Globals Window::globals = {1280,720,0,0,60,60};

Window::Window(WindowInit winit, void(*init)(void), void(*update)(double), void (*render)(void)){
    this->update = update;
    this->render = render;
    if(winit.ww != 0) Window::globals.ww = winit.ww;
    if(winit.wh != 0) Window::globals.wh = winit.wh;
    if(winit.tps != 0) Window::globals.tps = winit.tps;
    if(winit.fps != 0) Window::globals.fps = winit.fps;
    if(!glfwInit()){ // init glfw
        printf("Error: could not init glfw.\n");
        exit(1);
    }
    // what function to handle errors
    glfwSetErrorCallback(error_callback);
    // prefered and minimum openl gl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    // create window
    window = glfwCreateWindow(Window::globals.ww, Window::globals.wh, "Henlo Frens", NULL, NULL);
    if(window == NULL){
        printf("Error: could not create window.\n");
        glfwTerminate();
        exit(1);
    }
    // window callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    // use opengl with this window
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(0);//1 for vsync
    init();
}

void Window::Run(){
    // game loop
    double last_checkpoint = glfwGetTime();
    double last_update = glfwGetTime();
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
        if((now - last_checkpoint) * Window::globals.tps > update_ticks){
            glfwPollEvents();
            update(now - last_update);
            last_update = now;
            update_ticks++;
        }
        if((now - last_checkpoint) * Window::globals.fps < render_ticks)
            continue;
        render_ticks++;
        render();
        glfwSwapBuffers(window);
    }
}

void Window::End(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::error_callback(int error, const char* description){
    printf("Error: %s\n", description);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::window_size_callback(GLFWwindow* window, int w, int h){
    glfwGetFramebufferSize(window, &Window::globals.fbw, &Window::globals.fbh);
    glViewport(0, 0, Window::globals.fbw, Window::globals.fbh);
}
