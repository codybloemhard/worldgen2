#version 400
layout (location = 0) in vec3 vp;
uniform mat4 view;
uniform mat4 proj;
out vec3 pos;

void main(){
    pos = vp;
    gl_Position = proj * view * vec4(vp, 1.0);
}