#version 400
in vec3 pos;
out vec4 frag_colour;
uniform vec3 top_color;
uniform vec3 bot_color;
void main(){
    float h = normalize(pos).y;
    float t = pow(min(1, 1 - h), 5);
    float s = pow(min(1, 1 - h), 25);
    vec3 domecol = mix(top_color, bot_color, t);
    vec3 col = mix(domecol, vec3(0.5f,1f,1f), s);
    frag_colour = vec4(col, 1);
}