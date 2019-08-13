#ifndef worldstate_h
#define worldstate_h
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
    float world_scale;
    float walk_speed;
    float fly_speed;
};
#endif
