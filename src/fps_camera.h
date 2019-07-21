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
    void apply_vp(Shader*,bool usepos = true);
    float move_sens = 1.0f, mouse_sens = 0.1f,
        fov = 90.0f, aspect = 16.0f/9.0f,
        near = 0.1f, far = 1000.0f;
    glm::vec3 campos = glm::vec3(0.0f, 0.0f, 0.0f);
    private:
    float pitch, yaw, lastx, lasty;
    bool firstmouse = true;
    glm::vec3 camdir = glm::vec3(0.0f),
                camup = glm::vec3(0.0f),
                camri = glm::vec3(0.0f);
};
#endif