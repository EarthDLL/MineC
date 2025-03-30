#include "RenderTool.h"

#include <bitset>
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

void RenderTool ::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &RenderTool::init);
    ClassDB::bind_method(D_METHOD("get_render_data"), &RenderTool::get_render_data);
    ClassDB::bind_method(D_METHOD("render"), &RenderTool::render);
}

void RenderTool::init(){
    ResManager = Engine::get_singleton()->get_singleton("ResManager");
    block_infos = ResManager->get("block_infos");
    Array keys = block_infos.keys();
    for(int i = 0; i < keys.size() ; i++){
        short id = short(keys[i]);
        Object* object = block_infos.get(Variant(id),nullptr);
        BlockInfo* block_info = dynamic_cast<BlockInfo*>(object);
        if(block_info != nullptr && block_info -> cube_block){
            is_cube.set(id,false);
        }
    }
    is_cube.set(0,true);
    is_inited = true;
    return;
}
Array RenderTool :: get_render_data(){
    return render_data;
}

Vector3 RenderTool :: rotate_point(Vector3 pos , Vector3 rotate_pos , float degree){
    float rad = Math::deg_to_rad(degree);
    Vector3 translated_pos = pos - rotate_pos;
    Quaternion quat = Quaternion(Vector3(1,0,0).normalized(),rad);
    Basis basis = Basis(quat);
    Vector3 rotated_pos = basis.xform(translated_pos);
    Vector3 final_pos = rotated_pos + rotate_pos;
    return final_pos;
}

bool RenderTool::render(Ref<Chunk> chunk){
    if(!is_inited){
        return false;
    }
	vertice = PackedVector3Array();
	uv = PackedVector2Array();
	Array array = Array();
	array.resize(Mesh::ARRAY_MAX);

	for(int x = 0; x < chunk -> chunk_size.x ; x++){
        for(int y = 0; y < chunk -> chunk_size.y ; y++){
            for(int z = 0; z < chunk -> chunk_size.z ; z++){
                short block_id = chunk -> get_block(x,y,z);
                if(block_id != 0 && is_cube.test(block_id) == false){
                    short side = chunk -> get_block(x+1,y,z);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,z),block_id,3);
                    }
                    side = chunk -> get_block(x-1,y,z);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,z),block_id,2);
                    }
                    side = chunk -> get_block(x,y+1,z);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,z),block_id,0);
                    }
                    side = chunk -> get_block(x,y-1,z);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,z),block_id,1);
                    }
                    side = chunk -> get_block(x,y,z-1);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,z),block_id,4);
                    }
                    side = chunk -> get_block(x,y,z+1);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,z),block_id,5);
                    }
                }
            }
        }
    }

    array[Mesh::ARRAY_VERTEX] = vertice;
	array[Mesh::ARRAY_TEX_UV] = uv;
	render_data = array;
    //UtilityFunctions::print(count);
	return true;
}

void RenderTool::draw_cube_side(Vector3i pos,short block_id,int side){
    count++;
    Vector3 position = Vector3(0,0,0);
    
    Object* object = block_infos.get(block_id,nullptr);
    BlockInfo* info = dynamic_cast<BlockInfo*>(object);
    if(info == nullptr){
        return;
    }
    Dictionary texture_uv = info -> texture_uv;
    switch (side){
        case 0:
            position.y += 1;
            vertice.append(position + pos);
            position.x += 1;
            vertice.append(position + pos);
            position.z += 1;
            vertice.append(position + pos);
            vertice.append(position + pos);
            position.x -= 1;
            vertice.append(position + pos);
            position.z -= 1;
            vertice.append(position + pos);
            uv.append_array(PackedVector2Array(texture_uv.get("up",PackedVector2Array())));
            break;
        case 1:
            position.x += 1;
            position.z += 1;
            vertice.append(position + pos);
            position.z -= 1;
            vertice.append(position + pos);
            position.x -= 1;
            vertice.append(position + pos);
            vertice.append(position + pos);
            position.z += 1;
            vertice.append(position + pos);
            position.x += 1;
            vertice.append(position + pos);
            uv.append_array(PackedVector2Array(texture_uv.get("down",PackedVector2Array())));
            break;
        case 2:
            position.y += 1;
            vertice.append(position + pos);
            position.z += 1;
            vertice.append(position + pos);
            position.y -= 1;
            vertice.append(position + pos);
            vertice.append(position + pos);
            position.z -= 1;
            vertice.append(position + pos);
            position.y += 1;
            vertice.append(position + pos);
            uv.append_array(PackedVector2Array(texture_uv.get("west",PackedVector2Array())));
            break;
        case 3:
            position += Vector3(1,1,1);
            vertice.append(position + pos);
            position.z -= 1;
            vertice.append(position + pos);
            position.y -= 1;
            vertice.append(position + pos);
            vertice.append(position + pos);
            position.z += 1;
            vertice.append(position + pos);
            position.y += 1;
            vertice.append(position + pos);
            uv.append_array(PackedVector2Array(texture_uv.get("east",PackedVector2Array())));
            break;
        case 4:
            position += Vector3(1,1,0);
            vertice.append(position + pos);
            position.x -= 1;
            vertice.append(position + pos);
            position.y -= 1;
            vertice.append(position + pos);
            vertice.append(position + pos);
            position.x += 1;
            vertice.append(position + pos);
            position.y += 1;
            vertice.append(position + pos);
            uv.append_array(PackedVector2Array(texture_uv.get("north",PackedVector2Array())));
            break;
        case 5:
            position += Vector3(0,1,1);
            vertice.append(position + pos);
            position.x += 1;
            vertice.append(position + pos);
            position.y -= 1;
            vertice.append(position + pos);
            vertice.append(position + pos);
            position.x -= 1;
            vertice.append(position + pos);
            position.y += 1;
            vertice.append(position + pos);
            uv.append_array(PackedVector2Array(texture_uv.get("south",PackedVector2Array())));
            break;
    }
			
}