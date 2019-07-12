#include <stdio.h>
#include "/usr/include/GLFW/glfw3.h"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window, &globals.fbw, &globals.fbh);
    glViewport(0, 0, globals.fbw, globals.fbh);
    // game loop
    double last_time = glfwGetTime();
    while(!glfwWindowShouldClose(window)){
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
