#ifndef objects_h
#define objects_h
#include "shader.h"
#include "glstuff.h"
#include "fps_camera.h"
class Terrain{
    private:
    VAO *vao;
    SimplexNoise noise;
    Shader *shader;
    public:
    uint size = 640;
    float scale = 0.003f, height = 64.0f;
    Terrain(){
        noise = SimplexNoise();
        uint vert_len = (size+1)*(size+1);
        auto vertices = new Buffer(new float[vert_len * 3], vert_len * 3);
        for(int i = 0; i < vert_len; i++){
            uint x = (i % (size+1));
            uint y = (i / (size+1));
            vertices->data[i * 3 + 0] = x;
            float h = noise.fractal(5, x * scale, y * scale);
            h = h*0.5f+0.5f;
            h *= h;
            vertices->data[i * 3 + 1] = h * height;
            vertices->data[i * 3 + 2] = y;
        }
        uint indi_len = size * size * 6;
        auto indices = new Buffer(new uint[indi_len], indi_len);
        int i = 0;
        for(uint x = 0; x < size; x++)for(uint y = 0; y < size; y++){
            indices->data[i + 0] = (x + 0) + (y + 0) * (size + 1);
            indices->data[i + 1] = (x + 1) + (y + 0) * (size + 1);
            indices->data[i + 2] = (x + 1) + (y + 1) * (size + 1);
            indices->data[i + 3] = (x + 0) + (y + 0) * (size + 1);
            indices->data[i + 4] = (x + 1) + (y + 1) * (size + 1);
            indices->data[i + 5] = (x + 0) + (y + 1) * (size + 1);
            i += 6;
        }
        uint norm_len = vert_len;
        auto normals = new Buffer(new float[norm_len * 3], norm_len * 3);
        auto count = new uint[norm_len];
        auto norms = new glm::vec3[norm_len];
        for(uint x = 0; x < size; x++)for(uint y = 0; y < size; y++){
            auto r0 = (x + 0) + (y + 0) * (size + 1);
            auto r1 = (x + 1) + (y + 0) * (size + 1);
            auto r2 = (x + 1) + (y + 1) * (size + 1);
            auto r3 = (x + 0) + (y + 1) * (size + 1);
            auto p0 = from_arr(vertices->data, r0 * 3);
            auto p1 = from_arr(vertices->data, r1 * 3);
            auto p2 = from_arr(vertices->data, r2 * 3);
            auto p3 = from_arr(vertices->data, r3 * 3);
            auto pn = glm::normalize(glm::cross(p1-p0,p2-p0));
            auto qn = glm::normalize(glm::cross(p2-p0,p3-p0));
            count[r0] += 2;
            norms[r0] += pn + qn;
            count[r1]++;
            norms[r1] += pn;
            count[r2] += 2;
            norms[r2] += pn + qn;
            count[r3]++;
            norms[r3] += pn;
        }
        for(int i = 0; i < norm_len; i++){
            norms[i] /= (float)count[i];
        }
        for(int i = 0; i < norm_len; i++){
            normals->data[i * 3 + 0] = norms[i].x;
            normals->data[i * 3 + 1] = norms[i].y;
            normals->data[i * 3 + 2] = norms[i].z;
        }
        
        vao = new VAO();
        vao->bind();
        auto vbo = new GBO<float>(GL_ARRAY_BUFFER);
        vbo->bind();
        vbo->stuff(vertices);
        vbo->upload(GL_STATIC_DRAW);
        auto nbo = new GBO<float>(GL_ARRAY_BUFFER);
        nbo->bind();
        nbo->stuff(normals);
        nbo->upload(GL_STATIC_DRAW);
        auto ebo = new GBO<uint>(GL_ELEMENT_ARRAY_BUFFER);
        ebo->bind();
        ebo->stuff(indices);
        ebo->upload(GL_STATIC_DRAW);
        vao->add_ebo(ebo);
        add_vaa(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0);
        add_vaa(nbo, 1, 3, GL_FLOAT, GL_FALSE, 0);
        vao->unbind();
        shader = new Shader("shaders/terrain.vs", "shaders/terrain.fs");
    }
    void draw(FpsCamera *cam){
        shader->Use();
        shader->set_float("height", height);
        shader->set_float3("light_dir", 0.0f, -1.0f, 0.0f);
        cam->apply_mvp(shader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, size*size*6, GL_UNSIGNED_INT, 0);
    }
};
#endif