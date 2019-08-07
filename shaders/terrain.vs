#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 nr;
uniform vec4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float height;
uniform vec3 light_dir;
uniform float sea_level;

out VertexAttrib{
    vec3 vcol;
}vs_out;

float band(float value, float minh, float maxh, float edge_min, float edge_max){
    return smoothstep(minh - edge_min, maxh + edge_min, value) 
        * smoothstep(maxh + edge_max, maxh - edge_max, value);
}

vec3 mixlayer(vec3 nor, vec3 botl, vec3 topl, float maxtop, float softness){
    float power = clamp(dot(nor, vec3(0,-1,0)), 0, 1);
    power = smoothstep(maxtop, maxtop + softness, power);
    return mix(botl, topl, power);
}

void main(){
    vec4 p = vec4(vp, 1.0);
    p.x += model.x;
    p.z += model.y;
    gl_Position = proj * view * p;
    vec3 nor = normalize(nr);
    float h = p.y / height;
    vec3 sand = vec3(0.79f,0.58f,0.21f);
    vec3 grass = normalize(vec3(0.27f,0.48f,0.19f));
    vec3 plain = normalize(vec3(0.41f,0.22f,0.19f));
    vec3 stone = vec3(0.4f);
    vec3 moss = vec3(0.11f,0.27f,0.11f);
    vec3 snow = vec3(0.9f);
    
    float b_leak = 0.4f;
    float b_stop = 0.05f;
    float cband = 0, totalband = 0;
    vec3 col = vec3(0);
    cband = band(h, 0, sea_level, b_leak, b_leak); totalband += cband;
    col += cband * sand;
    cband = band(h, sea_level, 0.6, b_stop, b_leak); totalband += cband;
    col += cband * mixlayer(nor, plain, grass, 0.6, 0.4);
    cband = band(h, 0.6, 0.8, b_leak, b_stop); totalband += cband;
    col += cband * mixlayer(nor, stone, moss, 0.8, 0.1);
    cband = band(h, 0.8, 1.0, b_leak, b_leak); totalband += cband;
    col += cband * snow;
    col /= totalband;
    float lpow = max(dot(nor, light_dir), 0.0);
    col *= lpow;
    vs_out.vcol = col;
}
