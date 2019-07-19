#version 400
in vec3 pos;
out vec4 frag_colour;
void main(){
    float h = normalize(pos).y;
    float t = (h + 1.0f) / 2.0f;
    frag_colour = vec4(vec3(t), 0.5);
}