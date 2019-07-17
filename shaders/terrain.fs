#version 400
in vec3 pos;
out vec4 frag_colour;
void main(){
    frag_colour = vec4(vec3(0.2f + pos.y*0.8f), 1);
}