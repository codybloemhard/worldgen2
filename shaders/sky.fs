#version 400
in vec3 pos;
out vec4 frag_colour;
uniform vec3 top_color;
uniform vec3 bot_color;
uniform vec3 hor_color;
uniform float dome_sharpness;
uniform float horizon_sharpness;
void main(){
    float h = normalize(pos).y;
    float t = pow(min(1, 1 - h), dome_sharpness);
    float s = pow(min(1, 1 - h), horizon_sharpness);
    vec3 domecol = mix(top_color, bot_color, t);
    vec3 col = mix(domecol, hor_color, s);
    frag_colour = vec4(col, 1);
}