#version 400
in vec2 uv;

uniform sampler2D tex;
uniform vec3 color;

out vec4 frag_colour;

void main(){
    vec3 t = texture(tex, uv).xyz;
    frag_colour = vec4(t, 1.0);
}
