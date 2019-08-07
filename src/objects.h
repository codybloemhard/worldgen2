#ifndef objects_h
#define objects_h
#include "shader.h"
#include "glstuff.h"
#include "fps_camera.h"
#include "mathh.h"

class WorldState{
    private:
    WorldState(){}
    public:
    static WorldState& Get(){
        static WorldState inst;
        return inst;
    }
    WorldState(WorldState const&) = delete;
    void operator=(WorldState const&) = delete;
    //all variables
    glm::vec3 sun_dir;
    float sea_level;
    float world_height;
};
class TerrainPatch{
    private:
    VAO *vao;
    Shader *shader, *depshader;
    float offx, offy;
    public:
    uint size = 256;
    TerrainPatch(float scale, float offx, float offy){
        this->offx = offx;
        this->offy = offy;
        uint vert_len = (size+1)*(size+1);
        auto vertices = new Buffer(new float[vert_len * 3], vert_len * 3);
        for(int i = 0; i < vert_len; i++){
            uint x = (i % (size+1));
            uint y = (i / (size+1));
            vertices->data[i * 3 + 0] = x;
            float h = Mathh::terrain_noise(x + offx, y + offy, scale);
            vertices->data[i * 3 + 1] = h * WorldState::Get().world_height;
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
        shader->set_float4("model", glm::vec4(offx, offy, 0, 0));
        cam->apply_vp(shader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, size*size*6, GL_UNSIGNED_INT, 0);
    }
    void dep_draw(FpsCamera *cam){
        depshader->use();
        depshader->set_float3("campos", cam->campos);
        shader->set_float4("model", glm::vec4(offx, offy, 0, 0));
        cam->apply_vp(depshader);
        vao->bind();
        glDrawElements(GL_TRIANGLES, size*size*6, GL_UNSIGNED_INT, 0);
    }
};
class Terrain{
    private:
    float scale = 0.001f;
    TerrainPatch *lv0[2];
    public:
    Terrain(){
        lv0[0] = new TerrainPatch(scale, 0.0f, 0.0f);
        lv0[1] = new TerrainPatch(scale, 256.0f, 0.0f);
    }
    ~Terrain(){}
    void draw(FpsCamera *cam){
        for(TerrainPatch* tp : lv0)
            tp->draw(cam);
    }
    void dep_draw(FpsCamera *cam){
        for(TerrainPatch* tp : lv0)
            tp->dep_draw(cam);
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
