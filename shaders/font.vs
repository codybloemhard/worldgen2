#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 vuv;

uniform vec2 letter_scale;

out vec2 uv;

void main(){
    vec3 vertex = vp;
    vertex.x *= letter_scale.x;
    vertex.y *= letter_scale.y;
    gl_Position = vec4(vertex, 1.0);
    uv = vuv;
}
