#version 400
uniform vec4 colour;
uniform vec3 campos;
uniform vec3 light_dir;
uniform vec2 tex_size;
uniform sampler2D tex;
in vec3 pos;
out vec4 frag_colour;

float diff = 0.4;

void main(){
    vec4 col = colour;
    vec2 spos = gl_FragCoord.xy;
    spos.x = spos.x / tex_size.x;
    spos.y = spos.y / tex_size.y;
    float td = texture(tex, spos).x;
    float sd = distance(campos, pos) / 500;
    float seadep = abs(td - sd) * 3;
    float trans = clamp((1 - diff) + seadep, 0, col.w);

    float lpow = max(dot(vec3(0,-1,0), light_dir), 0.0);
    frag_colour = vec4(col.xyz * lpow, trans);
}
