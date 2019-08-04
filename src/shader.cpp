#include "shader.h"

Shader::Shader(const char* vs, const char* fs, const char* gs){
    std::ifstream ifvs(vs);
    std::string svs((std::istreambuf_iterator<char>(ifvs)),
                 std::istreambuf_iterator<char>());
    const char* rvs = svs.c_str();

    std::ifstream iffs(fs);
    std::string sfs((std::istreambuf_iterator<char>(iffs)),
                 std::istreambuf_iterator<char>());
    const char* rfs = sfs.c_str();

    std::string gstr;
    if(gs != nullptr){
        std::ifstream ifgs(gs);
        std::string sgs((std::istreambuf_iterator<char>(ifgs)),
                    std::istreambuf_iterator<char>());
        gstr = sgs;
    }
    const char* rgs = gstr.c_str();
    if(rgs != nullptr)
        printf("%s\n", rgs);

    Shader::vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Shader::vs, 1, &rvs, NULL);
    glCompileShader(Shader::vs);
    bool ok = check_error(Shader::vs, "Vertex");

    Shader::fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Shader::fs, 1, &rfs, NULL);
    glCompileShader(Shader::fs);
    ok &= check_error(Shader::fs, "Fragment");

    if(gs != nullptr){
        Shader::gs = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(Shader::gs, 1, &rgs, NULL);
        glCompileShader(Shader::gs);
        ok &= check_error(Shader::gs, "Geometry");
    }

    if(!ok){
        Shader::sh = 0;
        return;
    }

    Shader::sh = glCreateProgram();
    glAttachShader(Shader::sh, Shader::fs);
    glAttachShader(Shader::sh, Shader::vs);
    if(gs != nullptr)
        glAttachShader(Shader::sh, Shader::gs);
    glLinkProgram(Shader::sh);
    glDeleteShader(Shader::vs);
    glDeleteShader(Shader::fs);
    if(gs != nullptr)
        glDeleteShader(Shader::gs);
}

void Shader::use(){
    glUseProgram(sh);
}

void Shader::unuse(){
    glUseProgram(0);
}

void Shader::set_float(const char *name, const float &x){
    auto ul = uniloc(name);
    glUniform1f(ul, x);
}

void Shader::set_float2(const char *name, const float &x, const float &y){
    auto ul = uniloc(name);
    glUniform2f(ul, x, y);
}

void Shader::set_float3(const char *name, const float &x, const float &y, const float &z){
    auto ul = uniloc(name);
    glUniform3f(ul, x, y, z);
}

void Shader::set_float3(const char *name, const glm::vec3 &vec){
    auto ul = uniloc(name);
    glUniform3f(ul, vec.x, vec.y, vec.z);
}

void Shader::set_float4(const char *name, const float &x, const float &y, const float &z, const float &w){
    auto ul = uniloc(name);
    glUniform4f(ul, x, y, z, w);
}

void Shader::set_int(const char *name, const int &x){
    auto ul = uniloc(name);
    glUniform1i(ul, x);
}

void Shader::set_int2(const char *name, const int &x, const int &y){
    auto ul = uniloc(name);
    glUniform2i(ul, x, y);
}

void Shader::set_int3(const char *name, const int &x, const int &y, const int &z){
    auto ul = uniloc(name);
    glUniform3i(ul, x, y, z);
}

void Shader::set_int4(const char *name, const int &x, const int &y, const int &z, const int &w){
    auto ul = uniloc(name);
    glUniform4i(ul, x, y, z, w);
}

void Shader::set_uint(const char *name, const uint &x){
    auto ul = uniloc(name);
    glUniform1ui(ul, x);
}

void Shader::set_uint2(const char *name, const uint &x, const uint &y){
    auto ul = uniloc(name);
    glUniform2ui(ul, x, y);
}

void Shader::set_uint3(const char *name, const uint &x, const uint &y, const uint &z){
    auto ul = uniloc(name);
    glUniform3ui(ul, x, y, z);
}

void Shader::set_uint4(const char *name, const uint &x, const uint &y, const uint &z, const uint &w){
    auto ul = uniloc(name);
    glUniform4ui(ul, x, y, z, w);
}

void Shader::set_mat4(const char *name, const glm::mat4 &mat){
    auto ul = uniloc(name);
    glUniformMatrix4fv(ul, 1, GL_FALSE, &mat[0][0]);
}

bool Shader::check_error(GLuint shader, const char* typ){
    int succ;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succ);
    if(!succ){
        glGetShaderInfoLog(Shader::vs, 512, NULL, log);
        printf("Error: %s shader could not compile:\n", typ);
        printf(log);
        return false;
    }
    return true;
}

int Shader::uniloc(const char *name){
    int uniloc;
    auto it = unimap.find(name);
    if(it != unimap.end())
        uniloc = it->second;
    else
        uniloc = glGetUniformLocation(sh, name);
    return uniloc;
}
