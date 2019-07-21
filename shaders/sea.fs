#version 400
uniform vec4 colour;
uniform sampler2D tex;
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
    vec4 dcol = texture(tex, gl_FragCoord.xy);
    dcol.x = lindep(dcol.x);
    dcol.y = lindep(dcol.y);
    dcol.z = lindep(dcol.z);
    dcol.w = lindep(dcol.w);
    float e = lindep(gl_FragCoord.z);
    frag_colour = vec4(vec3(dcol.w / 500), 1);
}