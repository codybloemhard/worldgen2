class Mathh{
    public:
    //noise functions mapped to be between 0 and 1
    static float noise(float x, float y){
        float h = SimplexNoise::noise(x, y);
        return h*0.5f+0.5f;
    }
    static float noise(float x, float y, float z){
        float h = SimplexNoise::noise(x, y, z);
        return h*0.5f+0.5f;
    }
    static float terrain_noise(float x, float y, float scale){
        float h = Mathh::noise(x * scale, y * scale);
        h = h*h*h;
        h += Mathh::noise(x * scale * 4, y * scale * 4) * 0.3f;
        h += Mathh::noise(x * scale * 16, y * scale * 16) * 0.05f;
        h += Mathh::noise(x * scale * 64, y * scale * 64) * 0.005f;
        h += Mathh::noise(x * scale * 256, y * scale * 256) * 0.005f;
        return h / 1.36f;
    }
};
