#version 400
layout (location = 0) in vec3 vp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float height;

void main(){
    vec3 p = vp;
    p.x *= 10000.0f;
    p.z *= 10000.0f;
    p.y = height;
    gl_Position = proj * view * model * vec4(p, 1.0);
}