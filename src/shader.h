#ifndef shader_h
#define shader_h
#include "../deps/glad/glad.h"
#include "/usr/include/GLFW/glfw3.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <map>
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"

class Shader{
    public:
    Shader(const char* vs, const char* fs);
    void Use(), Unuse();
    void SetMat4(const char*, const glm::mat4 &mat);
    private:
    GLuint vs, fs, sh;
    std::map<const char*, int> unimap;
    enum Type { vertex, fragment };
    bool CheckError(GLuint shader, const char* typ);
};
#endif
