#version 400
in vec3 pos;
uniform vec3 campos;
out vec4 frag_colour;
void main(){
    float d = distance(campos, pos);
    frag_colour = vec4(vec3(d / 500), 1);
}