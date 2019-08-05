#version 400
in vec3 col;
out vec4 frag_colour;

void main(){
    frag_colour = vec4(col, 1);
}
