#version 400
layout (location = 0) in vec3 vp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 pos;

void main(){
    vec4 p = model * vec4(vp, 1.0);
    gl_Position = proj * view * p;
    pos = p.xyz;
}