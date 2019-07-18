#version 400
uniform vec4 colour;
out vec4 frag_colour;
void main(){
    frag_colour = normalize(colour);
}