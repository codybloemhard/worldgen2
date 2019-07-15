#ifndef window_h
#define window_h
#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include <math.h>
#include <stdio.h>

struct WindowInit{
    uint ww, wh, tps, fps;
};

struct Globals{
    uint ww, wh;
    int fbw, fbh;
    uint tps, fps;
};

class Window{
    public:
    Window(WindowInit, 
        void(*)(void),
        void(*)(float),
        void(*)(void),
        void(*)(GLFWwindow*,float),
        void(*)(GLFWwindow*,float,float),
        void(*)(void));
    void Run(), End();
    private:
    void (*update)(float);
    void (*render)(void), (*glexit)(void);
    void (*key_input)(GLFWwindow*,float);
    void (*mouse_input)(GLFWwindow*,float,float);
    static Globals globals;
    GLFWwindow* window;
    static float mousex, mousey;
    //callbacks
    static void error_callback(int, const char*);
    static void key_callback(GLFWwindow*, int, int, int, int);
    static void mouse_callback(GLFWwindow*, double, double);
    static void window_size_callback(GLFWwindow*, int, int);
};
#endif