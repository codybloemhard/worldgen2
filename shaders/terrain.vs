#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 nr;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float height;

out vec3 nor;
out vec3 col;

float band(float value, float minh, float maxh, float edge){
    return smoothstep(minh - edge, maxh + edge, value) * smoothstep(maxh + edge, maxh - edge, value);
}

vec3 mixlayer(vec3 botl, vec3 topl, float maxtop, float softness){
    float power = clamp(dot(nor, vec3(0,-1,0)), 0, 1);
    power = smoothstep(maxtop, maxtop + softness, power);
    return mix(botl, topl, power);
}

void main(){
    vec4 p = model * vec4(vp, 1.0);
    gl_Position = proj * view * p;
    nor = (model * vec4(nr, 1.0)).xyz;
    float h = p.y / height;
    vec3 sand = vec3(0.79f,0.58f,0.21f);
    vec3 grass = normalize(vec3(0.27f,0.48f,0.19f));
    vec3 plain = normalize(vec3(0.36f,0.22f,0.19f));
    vec3 snow = vec3(0.9f);
    
    float b = 0.09f;
    float cband = 0, totalband = 0;
    col = vec3(0);
    cband = band(h, 0, 0.25, b); totalband += cband;
    col += cband * sand;
    cband = band(h, 0.25, 0.6, b); totalband += cband;
    col += cband * mixlayer(plain, grass, 0.6, 0.4);
    cband = band(h, 0.6, 0.8, b); totalband += cband;
    col += cband * plain;
    cband = band(h, 0.8, 1.0, b); totalband += cband;
    col += cband * snow;
    col /= totalband;
}