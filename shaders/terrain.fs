#version 400
in vec3 pos;
out vec4 frag_colour;
void main(){
    frag_colour = vec4(normalize(vec3(0,pos.y,1-pos.y)), 1);
}