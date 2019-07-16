#ifndef fps_camera_h
#define fps_camera_h
#include <math.h>
#include "shader.h"
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"
#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
class FpsCamera{
    public:
    FpsCamera();
    void input(GLFWwindow*, float, float, float);
    void apply_mvp(Shader*);
    private:
    float pitch, yaw, lastx, lasty;
    glm::vec3 campos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camdir = glm::vec3(0.0f),
                camup = glm::vec3(0.0f),
                camri = glm::vec3(0.0f);
};
#endif