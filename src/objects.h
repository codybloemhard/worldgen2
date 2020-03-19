#ifndef objects_h
#define objects_h
#include "shader.h"
#include "glstuff.h"
#include "fps_camera.h"
#include "mathh.h"
#include <vector>
#include <stdlib.h> //rand,srand
#include <time.h> //time

uint clamp(float v, uint vsize){
    if(v >= vsize) return vsize - 1;
    if(v < 0) return 0;
    return v;
}

float min(float a, float b){
    if(a <= b) return a;
    return b;
}

float max(float a, float b){
    if(a >= b) return a;
    return b;
}

uint mapIndex(float x, float y, uint vsize){
    return (clamp(y, vsize) * (vsize + 1) + clamp(x, vsize)) * 3 + 1;
}

float maph(float x, float y, float* map, uint vsize){
    return map[mapIndex(x, y, vsize)];
}

void deposit(float x, float y, float amount, float* map, uint vsize){
    map[mapIndex(x, y, vsize)] += amount;
}

void erode(float x, float y, float amount, float* map, uint vsize){
    map[mapIndex(x, y, vsize)] -= amount;
}

class ErosionTerrain{
    private:
    VAO *vao;
    Shader *shader, *depshader;
    float size;
    uint vsize;
    public:
    ErosionTerrain(float size, uint vsize){
        this->size = size;
        this->vsize = vsize;
        uint vert_len = (vsize+1)*(vsize+1);
        auto vertices = new Buffer(new float[vert_len * 3], vert_len * 3);
        //vertices
        for(int i = 0; i < vert_len; i++){
            uint x = (i % (vsize+1));
            uint y = (i / (vsize+1));
            vertices->data[i * 3 + 0] = x;
            vertices->data[i * 3 + 1] = 0;
            vertices->data[i * 3 + 2] = y;
        }
        //middle kernel
        for(int i = 0; i < (vsize-1)*(vsize-1); i++){
            uint x = (i % (vsize-1)) + 1;
            uint y = (i / (vsize-1)) + 1;
            uint j = y * (vsize+1) + x;
            float h = Mathh::terrain_noise(x*size, y*size);
            vertices->data[j * 3 + 0] = x;
            vertices->data[j * 3 + 1] = h;
            vertices->data[j * 3 + 2] = y;
        }
        //erosion
        srand(time(NULL));
        int iters = 50000;
        uint max_path_len = vsize * 4;
        float pInertia = 0.2;
        float pMinSlope = 0.01f;
        float pCapacity = 3.0f;
        float pDeposition = 0.01f;
        float pErosion = 0.01f;
        float pGravity = 10.0f;
        float pEvaporation = 0.01f;
        float pRadius = 2.0f;
        float* map = vertices->data;
        float px, pz, dx, dz, vel, water, sediment;
        float radNor = pRadius * pRadius;
        for(int i = 0; i < iters; i++){
            px = rand() % vsize;
            pz = rand() % vsize;
            dx = dz = vel = sediment = 0;
            water = 1.0f;
            for(int j = 0; j < max_path_len; j++){
                float h00 = maph(px, pz, map, vsize);
                float h10 = maph(px + 1, pz, map, vsize);
                float h01 = maph(px, pz + 1, map, vsize);
                float h11 = maph(px + 1, pz + 1, map, vsize);
                float gx = h00+h01-h10-h11;
                float gz = h00+h10-h01-h11;
                float ndx = dx * pInertia - gx * (1.0f - pInertia);
                float ndz = dz * pInertia - gz * (1.0f - pInertia);
                float len = sqrt(ndx*ndx + ndz*ndz);
                if(len < 0.0001f){
                    ndx = (float)(rand() % 1000) / 1000.0f - 0.5f;
                    ndz = (float)(rand() % 1000) / 1000.0f - 0.5f;
                }
                ndx /= -len;
                ndz /= -len;
                float npx = px + ndx;
                float npz = pz + ndz;
                float nh = maph(npx, npz, map, vsize);
                float hdiff = nh - h00;
                float c = max(-hdiff, pMinSlope) * vel * water * pCapacity;
                if(sediment > c || hdiff > 0){
                    float dropAmount = 0.0f;
                    if(hdiff > 0) dropAmount = min(hdiff,sediment);
                    else dropAmount = (sediment - c) * pDeposition;
                    sediment -= dropAmount;
                    dropAmount /= 4;
                    // deposit(px, pz, dropAmount, map, vsize);
                    deposit(px + 1, pz, dropAmount, map, vsize);
                    deposit(px, pz + 1, dropAmount, map, vsize);
                    deposit(px - 1, pz, dropAmount, map, vsize);
                    deposit(px, pz - 1, dropAmount, map, vsize);
                }else{
                    float erodeAmount = min((c - sediment) * pErosion, -hdiff);
                    sediment += erodeAmount;
                    // erodeAmount /= radNor;
                    // for(int x = -pRadius; x < pRadius; x++)
                    //     for(int z = -pRadius; z < pRadius; z++)
                    //         erode(px + x, pz + z, erodeAmount, map, vsize);
                    erode(px,pz,erodeAmount, map, vsize);
                }
                float nvel = sqrt(vel * vel + hdiff * pGravity);
                float nwater = water * (1.0f - pEvaporation);
                px = npx;
                pz = npz;
                dx = ndx;
                dz = ndz;
                vel = nvel;
                water = nwater;
                if (water < 0.0001f) break;
            }
        }
        //indices
        uint indi_len = vsize * vsize * 6;
        auto indices = new Buffer(new uint[indi_len], indi_len);
        int i = 0;
        for(uint x = 0; x < vsize; x++)for(uint y = 0; y < vsize; y++){
            indices->data[i + 0] = (x + 0) + (y + 0) * (vsize + 1);
            indices->data[i + 1] = (x + 1) + (y + 0) * (vsize + 1);
            indices->data[i + 2] = (x + 1) + (y + 1) * (vsize + 1);
            indices->data[i + 3] = (x + 0) + (y + 0) * (vsize + 1);
            indices->data[i + 4] = (x + 1) + (y + 1) * (vsize + 1);
            indices->data[i + 5] = (x + 0) + (y + 1) * (vsize + 1);
            i += 6;
        }
        //normals
        uint norm_len = vert_len;
        auto normals = new Buffer(new float[norm_len * 3], norm_len * 3);
        auto count = new uint[norm_len];
        auto norms = new glm::vec3[norm_len];
        for(uint x = 0; x < vsize; x++)for(uint y = 0; y < vsize; y++){
            auto r0 = (x + 0) + (y + 0) * (vsize + 1);
            auto r1 = (x + 1) + (y + 0) * (vsize + 1);
            auto r2 = (x + 1) + (y + 1) * (vsize + 1);
            auto r3 = (x + 0) + (y + 1) * (vsize + 1);
            auto p0 = from_arr(vertices->data, r0 * 3);
            auto p1 = from_arr(vertices->data, r1 * 3);
            auto p2 = from_arr(vertices->data, r2 * 3);
            auto p3 = from_arr(vertices->data, r3 * 3);
            Mathh::scale_xz(p0, size);Mathh::scale_xz(p1, size);
            Mathh::scale_xz(p2, size);Mathh::scale_xz(p3, size);
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
        //gl stuff
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
        shader = new Shader("shaders/terrain.vs", "shaders/terrain.fs", "shaders/terrain.gs");
        depshader = new Shader("shaders/dep.vs", "shaders/dep.fs", nullptr);
    }
    ~ErosionTerrain(){
        delete vao;
        delete shader;
        delete depshader;
    }
    void draw(FpsCamera *cam){
        shader->use();
        shader->set_float("height", WorldState::Get().world_height);
        shader->set_float("sea_level", WorldState::Get().sea_level);
        shader->set_float3("light_dir", WorldState::Get().sun_dir);
        shader->set_float4("model", glm::vec4(0, 0, size, 0.0f));
        cam->apply_vp(shader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, vsize*vsize*6, GL_UNSIGNED_INT, 0);
    }
    void dep_draw(FpsCamera *cam){
        depshader->use();
        depshader->set_float3("campos", cam->campos);
        shader->set_float4("model", glm::vec4(0, 0, 0, 0));
        cam->apply_vp(depshader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, vsize*vsize*6, GL_UNSIGNED_INT, 0);
    }
};
class TerrainPatch{
    private:
    VAO *vao;
    Shader *shader, *depshader;
    float offx, offy, size;
    public:
    uint vsize;//YEET: need public?
    TerrainPatch(float offx, float offy, float size, uint vsize, uint subx, uint suby){
        this->offx = offx;
        this->offy = offy;
        this->size = size;
        this->vsize = vsize;
        uint vert_len = (vsize+1)*(vsize+1);
        auto vertices = new Buffer(new float[vert_len * 3], vert_len * 3);
        //vertices
        for(int i = 0; i < vert_len; i++){
            uint x = (i % (vsize+1));
            uint y = (i / (vsize+1));
            vertices->data[i * 3 + 0] = x;
            vertices->data[i * 3 + 1] = 0;
            vertices->data[i * 3 + 2] = y;
        }
        //middle kernel
        for(int i = 0; i < (vsize-1)*(vsize-1); i++){
            uint x = (i % (vsize-1)) + 1;
            uint y = (i / (vsize-1)) + 1;
            uint j = y * (vsize+1) + x;
            float h = Mathh::terrain_noise(x*size + offx, y*size + offy);
            vertices->data[j * 3 + 0] = x;
            vertices->data[j * 3 + 1] = h;
            vertices->data[j * 3 + 2] = y;
        }
        //YEET: only x rows have gaps
        //x rows kernel
        for(uint y = 0; y < vsize+1; y += vsize){
            for(uint x = 0; x < vsize+1; x++){
                uint j = y * (vsize+1) + x;
                float h;
                if((suby == 0 && y == 0) || (suby == 2 && y == vsize)){
                    uint div = ((subx * vsize) + x) % 3;
                    float b_power = ((float)div) / 3.0f;
                    float a = Mathh::terrain_noise(((int)x-(int)div)*size + offx, y*size + offy);
                    float b = Mathh::terrain_noise(((int)x-(int)div+3)*size + offx, y*size + offy);
                    h = (b_power * b) + ((1.0f - b_power)*a);
                    h = 0;
                }
                else
                    h = Mathh::terrain_noise(x*size + offx, y*size + offy);
                vertices->data[j * 3 + 0] = x;
                vertices->data[j * 3 + 1] = h;
                vertices->data[j * 3 + 2] = y;
            }
        }
        //y rows kernel
        /*for(uint x = 0; x < vsize+1; x += vsize){
            for(uint y = 0; y < vsize+1; y++){
                uint j = y * (vsize+1) + x;
                float h;
                if((subx == 0 && x == 0) || (subx == 2 && x == vsize)){
                    uint div = ((suby * vsize) + y) % 3;
                    float b_power = ((float)div) / 3.0f;
                    float a = Mathh::terrain_noise(x*size + offx, ((int)y-(int)div)*size + offy);
                    float b = Mathh::terrain_noise(x*size + offx, ((int)y-(int)div+3)*size + offy);
                    h = (b_power * b) + ((1.0f - b_power)*a);
                    h = 0;
                }
                else
                    h = Mathh::terrain_noise(x*size + offx, y*size + offy);
                vertices->data[j * 3 + 0] = x;
                vertices->data[j * 3 + 1] = h;
                vertices->data[j * 3 + 2] = y;
            }
        }*/
        //indices
        uint indi_len = vsize * vsize * 6;
        auto indices = new Buffer(new uint[indi_len], indi_len);
        int i = 0;
        for(uint x = 0; x < vsize; x++)for(uint y = 0; y < vsize; y++){
            indices->data[i + 0] = (x + 0) + (y + 0) * (vsize + 1);
            indices->data[i + 1] = (x + 1) + (y + 0) * (vsize + 1);
            indices->data[i + 2] = (x + 1) + (y + 1) * (vsize + 1);
            indices->data[i + 3] = (x + 0) + (y + 0) * (vsize + 1);
            indices->data[i + 4] = (x + 1) + (y + 1) * (vsize + 1);
            indices->data[i + 5] = (x + 0) + (y + 1) * (vsize + 1);
            i += 6;
        }
        //normals
        uint norm_len = vert_len;
        auto normals = new Buffer(new float[norm_len * 3], norm_len * 3);
        auto count = new uint[norm_len];
        auto norms = new glm::vec3[norm_len];
        for(uint x = 0; x < vsize; x++)for(uint y = 0; y < vsize; y++){
            auto r0 = (x + 0) + (y + 0) * (vsize + 1);
            auto r1 = (x + 1) + (y + 0) * (vsize + 1);
            auto r2 = (x + 1) + (y + 1) * (vsize + 1);
            auto r3 = (x + 0) + (y + 1) * (vsize + 1);
            auto p0 = from_arr(vertices->data, r0 * 3);
            auto p1 = from_arr(vertices->data, r1 * 3);
            auto p2 = from_arr(vertices->data, r2 * 3);
            auto p3 = from_arr(vertices->data, r3 * 3);
            Mathh::scale_xz(p0, size);Mathh::scale_xz(p1, size);
            Mathh::scale_xz(p2, size);Mathh::scale_xz(p3, size);
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
        //gl stuff
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
        shader = new Shader("shaders/terrain.vs", "shaders/terrain.fs", "shaders/terrain.gs");
        depshader = new Shader("shaders/dep.vs", "shaders/dep.fs", nullptr);
    }
    ~TerrainPatch(){
        delete vao;
        delete shader;
        delete depshader;
    }
    void draw(FpsCamera *cam){
        shader->use();
        shader->set_float("height", WorldState::Get().world_height);
        shader->set_float("sea_level", WorldState::Get().sea_level);
        shader->set_float3("light_dir", WorldState::Get().sun_dir);
        shader->set_float4("model", glm::vec4(offx, offy, size, 0.0f));
        cam->apply_vp(shader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, vsize*vsize*6, GL_UNSIGNED_INT, 0);
    }
    void dep_draw(FpsCamera *cam){
        depshader->use();
        depshader->set_float3("campos", cam->campos);
        shader->set_float4("model", glm::vec4(offx, offy, 0, 0));
        cam->apply_vp(depshader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, vsize*vsize*6, GL_UNSIGNED_INT, 0);
    }
};
class Terrain{
    private:
    uint vmulti = 16;
    std::vector<uint> levels { 2, 2, 2, 2, 2, 2, 2, 2, 1 };
    std::vector<std::vector<TerrainPatch*>> patches;
    public:
    Terrain(){
        uint vertices = 0;
        float psize = 64.0f;
        bool pring = false;
        for(uint lv : levels){
            std::vector<TerrainPatch*> ring;
            for(int i = 0; i < 9; i++)
                ring.push_back(nullptr);
            uint vsize = lv*vmulti;
            vertices += vsize * vsize;
            place_patches(ring, psize, vsize, pring);
            patches.push_back(ring);
            pring = true;
            psize *= 3;
        }
        printf("Terrain: %zu vertices.\n", vertices);
    }
    ~Terrain(){}
    void draw(FpsCamera *cam){
        for(auto ring : patches)
            for(auto patch : ring)
                if(patch != nullptr)
                    patch->draw(cam);
    }
    void dep_draw(FpsCamera *cam){
        for(auto ring : patches)
            for(auto patch : ring)
                if(patch != nullptr)
                    patch->dep_draw(cam);
    }
    private:
    void place_patches(std::vector<TerrainPatch*> &patches, float size, uint vsize, bool donutpatch){
        float relsize = size/(float)vsize;
        for(uint x = 0; x < 3; x++)
            for(uint y = 0; y < 3; y++){
                if(donutpatch && x == 1 && y == 1)
                    patches[y*3 + x] = nullptr;
                else
                    patches[y*3 + x] = new TerrainPatch(((float)x-1.5f) * size, ((float)y-1.5f) * size, relsize, vsize, x, y);
        }
    }
};
class Sea{
    private:
    VAO *vao;
    Shader *shader;
    float height;
    public:
    Sea(){
        this->height = WorldState::Get().world_height * WorldState::Get().sea_level;
        vao = new VAO();
        vao->bind();
        auto vertices = new Buffer(new float[12] {
            -1.0f, -1.0f, -1.0f,
            +1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,
        }, 12);
        auto indices = new Buffer(new uint[6] {
            0, 1, 2, 1, 3, 2
        }, 6);
        auto vbo = new GBO<float>(GL_ARRAY_BUFFER);
        vbo->bind();
        vbo->stuff(vertices);
        vbo->upload(GL_STATIC_DRAW);
        auto ebo = new GBO<uint>(GL_ELEMENT_ARRAY_BUFFER);
        ebo->bind();
        ebo->stuff(indices);
        ebo->upload(GL_STATIC_DRAW);

        vao->add_ebo(ebo);
        add_vaa(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0);
        vao->unbind();
        shader = new Shader("shaders/sea.vs", "shaders/sea.fs", nullptr);
    }
    ~Sea(){
        delete vao;
        delete shader;
    }
    void draw(FpsCamera *cam, GLuint deptex, uint ww, uint wh, float time){
        glBindTexture(GL_TEXTURE_2D, deptex);
        shader->use();
        shader->set_float4("colour", 0.2f, 0.2f, 0.7f, 0.9f);
        shader->set_float("height", height);
        shader->set_float("size", 1000.0f);
        shader->set_float3("campos", cam->campos);
        shader->set_float3("light_dir", WorldState::Get().sun_dir);
        shader->set_float2("tex_size", (float)ww, (float)wh);
        shader->set_float("time", time);
        shader->set_mat4("model", glm::mat4(1.0f));
        cam->apply_vp(shader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};
class Sky{
    private:
    VAO *vao;
    Shader *shader;
    public:
    glm::vec3 top_color, bot_color, horizon_color;
    float dome_sharpness, horizon_sharpness;
    Sky(){
        top_color = glm::vec3(0.2f,0.7f,0.8f);
        bot_color = glm::vec3(0.0f,1.0f,1.0f);
        horizon_color = glm::vec3(0.5f,1.0f,1.0f);
        dome_sharpness = 5;
        horizon_sharpness = 30;
        vao = new VAO();
        vao->bind();
        auto vertices = new Buffer(new float[24]{
            -1.0f, -1.0f, -1.0f,
            +1.0f, -1.0f, -1.0f,
            +1.0f, +1.0f, -1.0f,
            -1.0f, +1.0f, -1.0f,
            -1.0f, -1.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,
            +1.0f, +1.0f, +1.0f,
            -1.0f, +1.0f, +1.0f
        }, 24);
        auto indices = new Buffer(new uint[36]{
            0, 1, 3,
            3, 1, 2,
            1, 5, 2,
            2, 5, 6,
            5, 4, 6,
            6, 4, 7,
            4, 0, 7,
            7, 0, 3,
            3, 2, 7,
            7, 2, 6,
            4, 5, 0,
            0, 5, 1
        }, 36);
        auto vbo = new GBO<float>(GL_ARRAY_BUFFER);
        vbo->bind();
        vbo->stuff(vertices);
        vbo->upload(GL_STATIC_DRAW);
        auto ebo = new GBO<uint>(GL_ELEMENT_ARRAY_BUFFER);
        ebo->bind();
        ebo->stuff(indices);
        ebo->upload(GL_STATIC_DRAW);
        vao->add_ebo(ebo);
        add_vaa(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0);
        vao->unbind();
        shader = new Shader("shaders/sky.vs", "shaders/sky.fs", nullptr);
    }
    ~Sky(){
        delete vao;
        delete shader;
    }
    //needs to be drawn last
    void draw(FpsCamera *cam){
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        shader->use();
        shader->set_float3("top_color", top_color);
        shader->set_float3("bot_color", bot_color);
        shader->set_float3("hor_color", horizon_color);
        shader->set_float("dome_sharpness", dome_sharpness);
        shader->set_float("horizon_sharpness", horizon_sharpness);
        shader->set_float3("light_dir", WorldState::Get().sun_dir);
        shader->set_float3("sun_color", 1, 1, 1);
        shader->set_float("sun_power", 512);
        cam->apply_vp(shader, false);
        vao->bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }
};
#endif
