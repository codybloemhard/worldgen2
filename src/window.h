#ifndef window_h
#define window_h
#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include "../deps/linmath.h"
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
    Window(WindowInit, void(*)(void),void(*)(double), void(*)(void));
    void Run(), End();
    private:
    void (*update)(double);
    void (*render)(void);
    static Globals globals;
    GLFWwindow* window;
    //callbacks
    static void error_callback(int, const char*);
    static void key_callback(GLFWwindow*, int, int, int, int);
    static void window_size_callback(GLFWwindow*, int, int);
};
#endif