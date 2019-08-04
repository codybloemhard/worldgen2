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
    Shader(const char* vs, const char* fs, const char* gs);
    void use(), unuse();
    void set_float(const char*, const float&);
    void set_float2(const char*, const float&, const float&);
    void set_float3(const char*, const float&, const float&, const float&);
    void set_float3(const char*, const glm::vec3&);
    void set_float4(const char*, const float&, const float&, const float&, const float&);
    void set_int(const char*, const int&);
    void set_int2(const char*, const int&, const int&);
    void set_int3(const char*, const int&, const int&, const int&);
    void set_int4(const char*, const int&, const int&, const int&, const int&);
    void set_uint(const char*, const uint&);
    void set_uint2(const char*, const uint&, const uint&);
    void set_uint3(const char*, const uint&, const uint&, const uint&);
    void set_uint4(const char*, const uint&, const uint&, const uint&, const uint&);
    void set_mat4(const char*, const glm::mat4 &mat);
    private:
    GLuint vs, fs, gs, sh;
    std::map<const char*, int> unimap;
    enum Type { vertex, fragment };
    bool check_error(GLuint shader, const char* typ);
    int uniloc(const char*);
};
#endif
