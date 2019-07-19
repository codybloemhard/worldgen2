#ifndef glstuff_h
#define glstuff_h
#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include <vector>
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"

template <typename T>
class Buffer{
    public:
    Buffer(T* data, int l){
        this->data = data;
        this->l = l;
    }
    ~Buffer(){
        delete data;
    }
    int len(){
        return l;
    }
    int size(){
        return l * sizeof(T);
    }
    T* data;
    private:
    int l;
};
//General buffer object
template <typename T>
class GBO{
    public:
    GBO(GLenum target){
        glGenBuffers(1, &gbo);
        this->target = target;
    }
    ~GBO(){
        glDeleteBuffers(1, &gbo);
        delete data;
    }
    void bind(){
        glBindBuffer(target, gbo);
    }
    void stuff(Buffer<T> *data){
        this->data = data;
    }
    void upload(GLenum use){
        glBufferData(target, data->size(), data->data, use);
    }
    GLuint handle(){
        return gbo;
    }
    private:
    GLuint gbo;
    GLenum target;
    Buffer<T> *data;
};

template <typename T>
void add_vaa(GBO<T> *gbo, GLint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride){
    gbo->bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, NULL);
}

#define uint unsigned int

class VAO{
    public:
    VAO();
    ~VAO();
    void bind();
    void unbind();
    void add_ebo(GBO<uint>*);
    private:
    GLuint vao;
};

glm::vec3 from_arr(float *arr, uint n);

#endif