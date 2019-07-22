#version 400
uniform vec4 colour;
uniform vec3 campos;
uniform sampler2D tex;
in vec3 pos;
out vec4 frag_colour;

float near = 0.1; 
float far  = 5000.0; 

float lindep(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main(){
    vec4 col = colour;
    vec2 spos = gl_FragCoord.xy;
    spos.x = spos.x / 1600;
    spos.y = spos.y / 900;
    float td = texture(tex, spos).x;
    float sd = distance(campos, pos) / 500;
    float seadep = abs(td - sd) * 5;
    float trans;
    if(false) trans = col.w;
    else trans = clamp(0.5 + seadep, 0, col.w);
    frag_colour = vec4(col.xyz, trans);
    //frag_colour = vec4(vec3(td), 1);
}
