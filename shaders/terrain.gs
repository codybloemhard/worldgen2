#version 400
layout (triangles) in;
layout(triangle_strip, max_vertices=3) out;

in VertexAttrib{
    vec3 vcol;
}gs_in[];

out vec3 col;

void main(){
    vec3 avg = vec3(0);
    for(int i = 0; i < 3; i++){
        avg += gs_in[i].vcol;
    }
    avg /= 3;
    for(int i = 0; i < 3; i++){
        gl_Position = gl_in[i].gl_Position;
        col = avg;
        EmitVertex();
    }
    EndPrimitive();
}
