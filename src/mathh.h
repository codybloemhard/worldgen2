class Mathh{
    public:
    static float noise(float x, float y){
        float h = SimplexNoise::noise(x, y);
        return h*0.5f+0.5f;
    }  
};
