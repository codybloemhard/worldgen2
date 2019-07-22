#version 400
layout (location = 0) in vec3 vp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float height;
uniform float size;
out vec3 pos;
void main(){
    vec3 p = vp;
    p.x *= size;
    p.z *= size;
    p.y = height;
    vec4 _pos = model * vec4(p, 1.0);
    gl_Position = proj * view * _pos;
    pos = _pos.xyz;
}