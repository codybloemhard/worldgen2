#include "shader.h"

Shader::Shader(const char* vs, const char* fs){
    Shader::vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Shader::vs, 1, &vs, NULL);
    glCompileShader(Shader::vs);
    Shader::fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Shader::fs, 1, &fs, NULL);
    glCompileShader(Shader::fs);

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
