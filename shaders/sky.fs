#version 400
in float h;
out vec4 frag_colour;
void main(){
    float t = (h + 1.0f) / 2.0f;
    frag_colour = vec4(vec3(t), 0.5);
}