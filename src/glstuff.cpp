#include "glstuff.h"

//VBO

VBO::VBO(){
    glGenBuffers(1, &vbo);
}

VBO::~VBO(){
    glDeleteBuffers(1, &vbo);
}

void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VBO::stuff(Buffer<float> *data){
    this->data = data;
}

void VBO::upload(){
    glBufferData(GL_ARRAY_BUFFER, data->size(), data->data, GL_STATIC_DRAW);
}

GLuint VBO::handle(){
    return vbo;
}
