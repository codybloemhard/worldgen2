#include "shader.h"

Shader::Shader(const char* vs, const char* fs){
    std::ifstream ifvs(vs);
    std::string svs((std::istreambuf_iterator<char>(ifvs)),
                 std::istreambuf_iterator<char>());
    const char* rvs = svs.c_str();

    std::ifstream iffs(fs);
    std::string sfs((std::istreambuf_iterator<char>(iffs)),
                 std::istreambuf_iterator<char>());
    const char* rfs = sfs.c_str();

    Shader::vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Shader::vs, 1, &rvs, NULL);
    glCompileShader(Shader::vs);

    int succ;
    char log[512];
    glGetShaderiv(Shader::vs, GL_COMPILE_STATUS, &succ);
    if(!succ){
        glGetShaderInfoLog(Shader::vs, 512, NULL, log);
        printf("Error: Vertex shader could not compile:\n");
        printf(log);
    }

    Shader::fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Shader::fs, 1, &rfs, NULL);
    glCompileShader(Shader::fs);

    glGetShaderiv(Shader::fs, GL_COMPILE_STATUS, &succ);
    if(!succ){
        glGetShaderInfoLog(Shader::fs, 512, NULL, log);
        printf("Error: Fragment shader could not compile:\n");
        printf(log);
    }

    Shader::sh = glCreateProgram();
    glAttachShader(Shader::sh, Shader::fs);
    glAttachShader(Shader::sh, Shader::vs);
    glLinkProgram(Shader::sh);
    glDeleteShader(Shader::vs);
    glDeleteShader(Shader::fs);
}

void Shader::Use(){
    glUseProgram(Shader::sh);
}
