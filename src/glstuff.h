#ifndef glstuff_h
#define glstuff_h
#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"

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

class VAO{
    public:
    VAO();
    void bind();
    private:
    GLuint handle;
};

#endif