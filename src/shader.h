#ifndef shader_h
#define shader_h
#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include <string>
#include <fstream>
#include <streambuf>

class Shader{
    public:
    Shader(const char* vs, const char* fs);
    void Use();
    private:
    GLuint vs, fs, sh;
};
#endif
