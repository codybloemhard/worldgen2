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

void VAO::add_ebo(GBO<int>* ebo){
    ebo->bind();
}