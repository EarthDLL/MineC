#ifndef Chunk_H
#define Chunk_H

#include <bitset>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/noise.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <BlockModelInfo.h>

using namespace godot;

class Chunk : public RefCounted{
    GDCLASS(Chunk, RefCounted);

private:
    static void _bind_methods();
    bool is_full_block(Object* info);
public:
    static const int chunk_xz_size = 16;

    enum WorldType { 
	    NORMAL  = 0 ,
	    OLD = 1 ,
	    FLAT = 2 ,
	    CUSTOM = 3
    };

    Vector2i chunk_id = Vector2i(0,0);
    Vector3i chunk_size = Vector3i(16,256,16);
    int lowest_height = 0;
    int region_count = 0;
    unsigned short *blocks;
    bool is_loaded = false;
    void load_by_noise(const godot::Ref<godot::Noise>& noise);
    void init(Vector2i id,int lowest, int height);
    void set_block(int x , int y ,int z , short id);
    void set_block_high_range(int x , int z , int from_y , int to_y , int id);
    short get_block(int x , int y, int z);
    Vector2 get_center();
};

VARIANT_ENUM_CAST(Chunk :: WorldType);

#endif