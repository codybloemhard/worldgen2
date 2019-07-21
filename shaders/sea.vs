#version 400
layout (location = 0) in vec3 vp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float height;
uniform float size;

void main(){
    vec3 p = vp;
    p.x *= size;
    p.z *= size;
    p.y = height;
    vec4 glpos = 
    gl_Position = proj * view * model * vec4(p, 1.0);
}