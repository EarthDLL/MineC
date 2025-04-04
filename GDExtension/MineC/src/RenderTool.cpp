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
        BlockModelInfo* block_info = dynamic_cast<BlockModelInfo*>(object);
        if(block_info != nullptr && block_info -> is_cube){
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

void RenderTool::render(Ref<Chunk> chunk){
    render_data = Array();
    chunk_pos = Vector3(chunk -> chunk_id.x * 16 , chunk -> lowest_height , chunk -> chunk_id.y * 16);
    for(int i = 0; i< chunk -> chunk_size.y / 32;i++){
        render_region(i , chunk);
    }
    return;
}

void RenderTool::render_edge(Ref<Chunk> chunk , int side , Ref<Chunk> edge_chunk){
    if(side < 2 || side > 5){
        return;
    }
    /*Side说明
        2-west 3-east 4-north 5-south
    */

    render_data = Array();
    chunk_pos = Vector3(chunk -> chunk_id.x * 16 , chunk -> lowest_height , chunk -> chunk_id.y * 16);
    for(int i = 0; i< chunk -> chunk_size.y / 32;i++){
        render_edge_region(i , chunk ,side , edge_chunk);
    }
    return;
}

bool RenderTool::render_edge_region(int index , Ref<Chunk> chunk , int side , Ref<Chunk> edge_chunk){
    if(!is_inited){
        return false;
    }
	vertice = PackedVector3Array();
	uv = PackedVector2Array();
	Array array = Array();
    switch(side){
        case 2:
            for(int y = index * 32; y < (index+1) * 32 ; y++){
                for(int z = 0; z < 16 ; z++){
                    short block_id = chunk -> get_block(0,y,z);
                    short side = edge_chunk -> get_block(15,y,z);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(0,y,z),block_id,2);
                    }
                }
            }
            break;
        case 3:
            for(int y = index * 32; y < (index+1) * 32 ; y++){
                for(int z = 0; z < 16 ; z++){
                    short block_id = chunk -> get_block(15,y,z);
                    short side = edge_chunk -> get_block(0,y,z);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(15,y,z),block_id,3);
                    }
                }
            }
            break;
        case 4:
            for(int y = index * 32; y < (index+1) * 32 ; y++){
                for(int x = 0; x < 16 ; x++){
                    short block_id = chunk -> get_block(x,y,0);
                    short side = edge_chunk -> get_block(x,y,15);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,0),block_id,4);
                    }
                }
            }
            break;
        case 5:
            for(int y = index * 32; y < (index+1) * 32 ; y++){
                for(int x = 0; x < 16 ; x++){
                    short block_id = chunk -> get_block(x,y,15);
                    short side = edge_chunk -> get_block(x,y,0);
                    if(is_cube.test(side) == true){
                        draw_cube_side(Vector3i(x,y,15),block_id,5);
                    }
                }
            }
            break;
    }
    array.append(vertice);
	array.append(uv);
	render_data.append(array);
    return true;
}

bool RenderTool::render_region(int index, Ref<Chunk> chunk){
    if(!is_inited){
        return false;
    }
	vertice = PackedVector3Array();
	uv = PackedVector2Array();
	Array array = Array();

	for(int x = 0; x < 16 ; x++){
        for(int y = index * 32; y < (index+1) * 32 ; y++){
            for(int z = 0; z < 16 ; z++){
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

    array.append(vertice);
	array.append(uv);
	render_data.append(array);
    //UtilityFunctions::print(count);
	return true;
}

void RenderTool::draw_cube_side(Vector3i pos,short block_id,int side){
    Vector3 position = chunk_pos;
    
    Object* object = block_infos.get(block_id,nullptr);
    BlockModelInfo* info = dynamic_cast<BlockModelInfo*>(object);
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