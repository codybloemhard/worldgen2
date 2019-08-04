#version 400
uniform vec3 light_dir;
in vec3 nor;
in vec3 col;
out vec4 frag_colour;

void main(){
    float lpow = max(dot(nor, light_dir), 0.0);
    frag_colour = vec4(col * lpow, 1);
}
