#version 400
uniform float height;
in vec3 pos;
out vec4 frag_colour;
void main(){
    vec3 p = pos;
    p.y /= height;
    vec3 sand = vec3(0.79f,0.58f,0.21f);
    vec3 grass = vec3(0.27f,0.48f,0.19f);
    vec3 plain = vec3(0.36f,0.22f,0.19f);
    vec3 snow = vec3(0.9f);

    float b = 0.05f;
    vec3 col;
    col += smoothstep(0.0f-b, 0.2f+b, p.y) * smoothstep(0.2f+b, 0.2f-b, p.y) * sand;
    col += smoothstep(0.2f-b, 0.6f+b, p.y) * smoothstep(0.6f+b, 0.6f-b, p.y) * grass;
    col += smoothstep(0.6f-b, 0.8f+b, p.y) * smoothstep(0.8f+b, 0.8f-b, p.y) * plain;
    col += smoothstep(0.8f-b, 1.0f+b, p.y) * smoothstep(1.0f+b, 1.0f-b, p.y) * snow;
    col = normalize(col);
    frag_colour = vec4(col, 1);
}