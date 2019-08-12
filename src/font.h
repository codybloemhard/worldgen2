#ifndef font_h
#define font_h
#include "../deps/stb_image.h"
#include <math.h> //fmod
class Font{
    private:
    uint tex;
    Shader *shader;
    VAO *vao;
    GBO<float> *vbo, *tbo;
    GBO<uint> *ebo;
    Buffer<float> *uvs;
    public:
    Font(){
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        int width, height, nrChannels;
        //stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load("assets/codyfont.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            printf("Error: Could not load font image.\n");
        stbi_image_free(data);
        vao = new VAO();
        vao->bind();
        vbo = new GBO<float>(GL_ARRAY_BUFFER);
        auto vertices = new Buffer(new float[256*12], 256*12);
        for(int i = 0; i < 256; i++){
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
        uvs = new Buffer(new float[256*8], 256*8);
        for(int i = 0; i < 256; i++){
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
        auto indices = new Buffer(new uint[256*6], 256*6);
        for(uint i = 0; i < 256; i++){
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
    void print(const char* msg){
        int i = 0;
        while(i < 256){
            char chr = msg[i];
            if(chr == 0) break;
            if(chr < 32 || chr > 127){
                printf("Font: char nr %d is not in range.\n");
                chr = 0;
            }
            else
                chr -= 32;
            
            float xstep = 1.0f / 12.0f;
            float ystep = 1.0f / 8.0f;
            int xx = (chr % 12);
            int yy = (chr / 12);
            float x = (float)xx / 12.0f;
            float y = (float)yy / 8.0f;

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
        float letter_width = 0.01f;
        float letter_height = letter_width * (12.0f/8.0f) * (16.0f/9.0f);
        shader->set_float2("letter_scale", letter_width, letter_height);
        shader->set_float3("color", 1.0f, 0.0f, 0.0f);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, tex);
        vao->bind();
        tbo->bind();
        tbo->reupload();
        glDrawElements(GL_TRIANGLES, 256*6, GL_UNSIGNED_INT, 0);
        glEnable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
#endif
