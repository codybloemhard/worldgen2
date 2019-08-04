#version 400
layout (triangles) in;
layout(triangle_strip, max_vertices=3) out;

in VertexAttrib{
    vec3 vcol;
    vec3 vnor;
}gs_in[];

out vec3 nor;
out vec3 col;

void main(){
    for(int i = 0; i < 3; i ++){
        gl_Position = gl_in[i].gl_Position;
        nor = gs_in[i].vnor;
        col = gs_in[i].vcol;
        EmitVertex();
    }
    EndPrimitive();
}
