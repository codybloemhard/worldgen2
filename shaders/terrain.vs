#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 nr;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float height;

out vec3 pos;
out vec3 nor;

void main(){
    vec4 p = model * vec4(vp, 1.0);
    gl_Position = proj * view * p;
    pos = p.xyz;
    nor = nr;//(model * vec4(nr, 1.0)).xyz;
}