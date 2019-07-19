#include "glstuff.h"

VAO::VAO(){
    glGenVertexArrays(1, &vao);
}

VAO::~VAO(){
    glDeleteVertexArrays(1, &vao);
}

void VAO::bind(){
    glBindVertexArray(vao);
}
    
void VAO::unbind(){
    glBindVertexArray(0);
}

void VAO::add_ebo(GBO<uint>* ebo){
    ebo->bind();
}

glm::vec3 from_arr(float *arr, uint n){
    return glm::vec3(arr[n+0],arr[n+1],arr[n+2]);
}
