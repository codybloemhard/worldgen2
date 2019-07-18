#version 400
layout (location = 0) in vec3 vp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float height;

out vec3 pos;

void main(){
    vec3 copy = vp;
    copy.y *= height;
    vec4 p = model * vec4(copy, 1.0);
    gl_Position = proj * view * p;
    pos = p.xyz;
}