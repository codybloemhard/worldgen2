#version 400
in vec2 uv;

uniform sampler2D tex;
uniform vec3 color;

out vec4 frag_colour;

void main(){
    float t = texture(tex, uv).x;
    frag_colour = vec4(color, t);
}
