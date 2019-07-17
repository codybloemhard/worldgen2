#include "fps_camera.h"

FpsCamera::FpsCamera(){}

void FpsCamera::input(GLFWwindow *window, float elaps, float xpos, float ypos){
    auto xzdir = camdir;
    xzdir.y = 0.0f;
    xzdir = glm::normalize(xzdir);
    auto xzri = camri;
    xzri.y = 0.0f;
    xzri = glm::normalize(xzri);
    auto up = glm::vec3(0.0f,1.0f,0.0f);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        campos += xzdir * elaps * move_sens;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        campos -= xzdir * elaps * move_sens;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        campos -= xzri * elaps * move_sens;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        campos += xzri * elaps * move_sens;
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        campos += up * elaps * move_sens;
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        campos -= up * elaps * move_sens;
    
    float dx = xpos - lastx;
    float dy = lasty - ypos;
    lastx = xpos;
    lasty = ypos;
    yaw += dx * mouse_sens;
    pitch += dy * mouse_sens;
    
    if(pitch > 89.99f)
        pitch = 89.99f;
    if(pitch < -89.99f)
        pitch = -89.99f;
}

void FpsCamera::apply_mvp(Shader *shader){
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    camdir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir.y = sin(glm::radians(pitch));
    camdir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir = glm::normalize(camdir);
    camri = glm::normalize(glm::cross(camdir, up));
    camup = glm::normalize(glm::cross(camri, camdir));
    
    glm::mat4 view = glm::lookAt(campos, campos + camdir, camup);
    glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, near, far);
    shader->SetMat4("model", glm::mat4(1.0f));
    shader->SetMat4("view", view);
    shader->SetMat4("proj", proj);
}
