#ifndef RenderTool_H
#define RenderTool_H

#include <bitset>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/basis.hpp>
#include <Chunk.h>
#include <BlockInfo.h>

using namespace godot;

class RenderTool : public RefCounted{
    GDCLASS(RenderTool, RefCounted);

private:
    static void _bind_methods();
    std::bitset<65536> is_cube;
    //0 - 完整方块 1 - 不完整方块
    Dictionary block_infos;
    bool is_inited = false;
    PackedVector3Array vertice;
	PackedVector2Array uv;
public:
    int count = 0;
    Array render_data;
    Array get_render_data();
    void init();
    Object *ResManager;
    bool render(Ref<Chunk> chunk);
    void draw_cube_side(Vector3i pos,short block_id,int side);
    Vector3 rotate_point(Vector3 pos , Vector3 rotate_pos , float degree);
};


#endif