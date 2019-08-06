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
};
