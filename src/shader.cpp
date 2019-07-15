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

    bool ok = CheckError(Shader::vs, "Vertex");

    Shader::fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Shader::fs, 1, &rfs, NULL);
    glCompileShader(Shader::fs);

    ok &= CheckError(Shader::fs, "Fragment");
    if(!ok){
        Shader::sh = 0;
        return;
    }

    Shader::sh = glCreateProgram();
    glAttachShader(Shader::sh, Shader::fs);
    glAttachShader(Shader::sh, Shader::vs);
    glLinkProgram(Shader::sh);
    glDeleteShader(Shader::vs);
    glDeleteShader(Shader::fs);
}

void Shader::Use(){
    glUseProgram(sh);
}

void Shader::Unuse(){
    glUseProgram(0);
}

void Shader::SetMat4(const char* name, const glm::mat4 &mat){
    int uniloc = glGetUniformLocation(sh, name);
    glUniformMatrix4fv(uniloc, 1, GL_FALSE, &mat[0][0]);
}

bool Shader::CheckError(GLuint shader, const char* typ){
    int succ;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succ);
    if(!succ){
        glGetShaderInfoLog(Shader::vs, 512, NULL, log);
        printf("Error: %c shader could not compile:\n", typ);
        printf(log);
        return false;
    }
    return true;
}
