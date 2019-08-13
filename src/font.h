#ifndef font_h
#define font_h
#include "../deps/stb_image.h"
#include <math.h> //fmod
#include "mathh.h"
class Font{
    private:
    uint tex;
    Shader *shader;
    VAO *vao;
    GBO<float> *vbo, *tbo;
    GBO<uint> *ebo;
    Buffer<float> *uvs;
    const uint max_len = 256;
    const uint framesx, framesy;
    float aspect_ratio;
    public:
    Font(const char *file, uint _framesx, uint _framesy, float aspect_ratio)
    : framesx(_framesx), framesy(_framesy){
        this->aspect_ratio = aspect_ratio;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        int width, height, nrChannels;
        unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
        if (data)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            printf("Error: Could not load font image.\n");
        stbi_image_free(data);
        vao = new VAO();
        vao->bind();
        vbo = new GBO<float>(GL_ARRAY_BUFFER);
        auto vertices = new Buffer(new float[max_len*12], max_len*12);
        for(int i = 0; i < max_len; i++){
            vertices->data[i*12 +  0] = i;
            vertices->data[i*12 +  1] = 0;
            vertices->data[i*12 +  2] = 1;

            vertices->data[i*12 +  3] = i + 1;
            vertices->data[i*12 +  4] = 0;
            vertices->data[i*12 +  5] = 1;

            vertices->data[i*12 +  6] = i + 1;
            vertices->data[i*12 +  7] = 1;
            vertices->data[i*12 +  8] = 1;

            vertices->data[i*12 +  9] = i;
            vertices->data[i*12 + 10] = 1;
            vertices->data[i*12 + 11] = 1;
        }
        vbo->stuff(vertices);
        vbo->bind();
        vbo->upload(GL_STATIC_DRAW);
        tbo = new GBO<float>(GL_ARRAY_BUFFER);
        uvs = new Buffer(new float[max_len*8], max_len*8);
        for(int i = 0; i < max_len; i++){
            uvs->data[i*8 + 0] = 0.0f;
            uvs->data[i*8 + 1] = 0.0f;

            uvs->data[i*8 + 2] = 0.0f;
            uvs->data[i*8 + 3] = 0.0f;

            uvs->data[i*8 + 4] = 0.0f;
            uvs->data[i*8 + 5] = 0.0f;

            uvs->data[i*8 + 6] = 0.0f;
            uvs->data[i*8 + 7] = 0.0f;
        }
        tbo->stuff(uvs);
        tbo->bind();
        tbo->upload(GL_STATIC_DRAW);
        ebo = new GBO<uint>(GL_ELEMENT_ARRAY_BUFFER);
        auto indices = new Buffer(new uint[max_len*6], max_len*6);
        for(uint i = 0; i < max_len; i++){
            uint off = i * 4;
            indices->data[i*6 + 0] = 0 + off;
            indices->data[i*6 + 1] = 1 + off;
            indices->data[i*6 + 2] = 2 + off;

            indices->data[i*6 + 3] = 0 + off;
            indices->data[i*6 + 4] = 2 + off;
            indices->data[i*6 + 5] = 3 + off;
        }
        ebo->stuff(indices);
        ebo->bind();
        ebo->upload(GL_STATIC_DRAW);
        vao->add_ebo(ebo);
        add_vaa(vbo, 0, 3, GL_FLOAT, GL_FALSE, 0);
        add_vaa(tbo, 1, 2, GL_FLOAT, GL_FALSE, 0);
        vao->unbind();
        shader = new Shader("shaders/font.vs", "shaders/font.fs", nullptr);
    }
    ~Font(){
        glDeleteTextures(1, &tex);
        delete shader;
        delete vbo;
        delete tbo;
        delete ebo;
    }
    void print(const char* msg, float letter_width, const glm::vec3 color, const glm::vec2 pos){
        float xstep = 1.0f / (float)framesx;
        float ystep = 1.0f / (float)framesy;
        int i = 0;
        while(i < max_len){
            char chr = msg[i];
            if(chr == 0) break;
            if(chr < 32 || chr > 127){
                printf("Font: char nr %d is not in range.\n");
                chr = 0;
            }
            else
                chr -= 32;
            
            int xx = (chr % framesx);
            int yy = (chr / framesx);
            float x = (float)xx / (float)framesx;
            float y = (float)yy / (float)framesy;

            uvs->data[i*8 + 0] = x;
            uvs->data[i*8 + 1] = y + ystep;

            uvs->data[i*8 + 2] = x + xstep;
            uvs->data[i*8 + 3] = y + ystep;

            uvs->data[i*8 + 4] = x + xstep;
            uvs->data[i*8 + 5] = y;

            uvs->data[i*8 + 6] = x;
            uvs->data[i*8 + 7] = y;
            i++;
        }
        shader->use();
        float letter_height = letter_width * ((float)framesx/(float)framesy) * aspect_ratio;
        shader->set_float2("pos", pos.x, pos.y - letter_height);//pos of topleft corner first letter
        shader->set_float2("letter_scale", letter_width, letter_height);
        shader->set_float3("color", color);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, tex);
        vao->bind();
        tbo->bind();
        tbo->reupload();
        glDrawElements(GL_TRIANGLES, max_len*6, GL_UNSIGNED_INT, 0);
        glEnable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
#endif
