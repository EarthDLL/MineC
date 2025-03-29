#ifndef Chunk_H
#define Chunk_H

#include <bitset>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/noise.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <BlockInfo.h>

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
    int region_count = 0;
    unsigned short *blocks;
    int blocks_length;
    Dictionary render_data;
    bool is_loaded = false;
    PackedVector3Array vertice = PackedVector3Array();
    PackedVector2Array uv = PackedVector2Array();

    Dictionary get_render_data() const;
    void set_render_data(const Dictionary value);
    bool render(int region_index);
    void load_by_noise(const godot::Ref<godot::Noise>& noise);
    void init(Vector2i id, Vector3i size);
    void set_block(int x , int y ,int z , short id);
    void set_block_high_range(int x , int z , int from_y , int to_y , int id);
    short get_block(int x , int y, int z);
    void draw_cube_side(Vector3i pos,Ref<BlockInfo> block,int side);
};

VARIANT_ENUM_CAST(Chunk :: WorldType);

#endif