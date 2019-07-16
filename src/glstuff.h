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

class VBO{
    public:
    VBO();
    ~VBO();
    void bind();
    void stuff(Buffer<float>*);
    void upload();
    GLuint handle();
    private:
    GLuint vbo;
    Buffer<float> *data;
};

class VAO{
    public:
    VAO();
    void bind();
    private:
    GLuint handle;
};

#endif