#include "Chunk.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <BlockInfo.h>

using namespace godot;

const int Chunk::chunk_xz_size;

void Chunk::init(Vector2i id = Vector2i(0,0) , Vector3i size = Vector3i(16,256,16)) {
	chunk_id = id;
    chunk_size = size;
}

void Chunk::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "id","size"), &Chunk::init);
    ClassDB::bind_method(D_METHOD("load_by_noise","noise"), &Chunk::load_by_noise);
    ClassDB::bind_method(D_METHOD("render", "region_index"), &Chunk::render);
    ClassDB::bind_method(D_METHOD("get_render_data"), &Chunk::get_render_data);
    ClassDB::bind_method(D_METHOD("set_render_data","value"), &Chunk::set_render_data);
    ClassDB::add_property("Chunk", PropertyInfo(Variant::DICTIONARY, "render_data"), "set_render_data", "get_render_data");
}

Dictionary Chunk :: get_render_data() const{
    return render_data;
}
void Chunk :: set_render_data(const Dictionary value){
    render_data = value;
}

void Chunk::load_by_noise(const godot::Ref<godot::Noise>& noise){
    Object* debuger = Engine::get_singleton()->get_singleton("Debuger");
    //debuger->callv("output_time",Array());
    blocks = new unsigned short[chunk_size.x * chunk_size.y * chunk_size.z];
    std::fill(blocks, blocks + (chunk_size.x * chunk_size.y * chunk_size.z), 0);
    Vector2i start_point = chunk_id * chunk_xz_size;
	for(int x=0 ; x < chunk_size.x ; x++){
        for(int y=0 ; y < chunk_size.z ; y++){
                float noise_high = noise -> get_noise_2d(start_point.x + x , start_point.y + y);
                int high = std::round((std::clamp(noise_high+0.8,-1.0,1.0)+1) *32);
                set_block_high_range(x,y,0,high,1);
        }
    }
	is_loaded = true;
    //debuger->callv("output_time",Array());
}

void Chunk::set_block(int x , int y ,int z , short id){
    int index = y * chunk_size.x * chunk_size.z + chunk_size.x * z + x;
    blocks[index] = id;
}

short Chunk::get_block(int x , int y , int z){
    if(x < 0 || x >= chunk_size.x || z < 0 || z >= chunk_size.z || y < 0 || y >= chunk_size.y){
        return 1;
    }
    int index = y * chunk_size.x * chunk_size.z + chunk_size.x * z + x;
    return blocks[index];
}

void Chunk::set_block_high_range(int x , int z , int from_y , int to_y , int id){
    if(from_y > to_y){
        return;
    }
    int index = from_y * chunk_size.x * chunk_size.z + chunk_size.x * z + x;
    blocks[index] = id;
    for(int y= from_y ; y < to_y; y++){
        index += chunk_size.x * chunk_size.z;
        blocks[index] = id;
    }
}

Vector2 Chunk::get_center(){
    return Vector2(chunk_id) * 16 + Vector2(8,8);
}

bool Chunk :: is_full_block(Object *info){
    if(info == nullptr){
        return false;
    }
    return info->get("is_cube");
}

	
bool Chunk::render(int region_index){
    if(!is_loaded){
        return false;
    }
    Object *ResManager = Engine::get_singleton()->get_singleton("ResManager");
    Dictionary block_infos = ResManager->get("block_infos");
	vertice = PackedVector3Array();
	uv = PackedVector2Array();
	Array array = Array();
	array.resize(Mesh::ARRAY_MAX);

	for(int x = 0; x < chunk_size.x ; x++){
        for(int y = region_index * 16; y < chunk_size.y ; y++){
            for(int z = 0; z < chunk_size.z ; z++){
                Ref<BlockInfo> block = block_infos.get(Variant(get_block(x,y,z)),nullptr);
                if(block != nullptr){
                    short side = get_block(x+1,y,z);
                    if(side == 0){
                        draw_cube_side(Vector3i(x,y,z),block,3);
                    }
                    side = get_block(x-1,y,z);
                    if(side == 0){
                        draw_cube_side(Vector3i(x,y,z),block,2);
                    }
                    side = get_block(x,y+1,z);
                    if(side == 0){
                        draw_cube_side(Vector3i(x,y,z),block,0);
                    }
                    side = get_block(x,y-1,z);
                    if(side == 0){
                        draw_cube_side(Vector3i(x,y,z),block,1);
                    }
                    side = get_block(x,y,z-1);
                    if(side == 0){
                        draw_cube_side(Vector3i(x,y,z),block,4);
                    }
                    side = get_block(x,y,z+1);
                    if(side == 0){
                        draw_cube_side(Vector3i(x,y,z),block,5);
                    }
                }
            }
        }
    }

    array[Mesh::ARRAY_VERTEX] = vertice;
	array[Mesh::ARRAY_TEX_UV] = uv;
	render_data[region_index] = array;
	return true;
}

void Chunk::draw_cube_side(Vector3i pos,Ref<BlockInfo> block,int side){
    Vector3 position = Vector3(pos);
    Dictionary texture_uv = block->texture_uv;
    switch (side){
        case 0:
            position.y += 1;
            vertice.append(position);
            position.x += 1;
            vertice.append(position);
            position.z += 1;
            vertice.append(position);
            vertice.append(position);
            position.x -= 1;
            vertice.append(position);
            position.z -= 1;
            vertice.append(position);
            uv.append_array(PackedVector2Array(texture_uv.get("up",PackedVector2Array())));
            break;
        case 1:
            position.x += 1;
            position.z += 1;
            vertice.append(position);
            position.z -= 1;
            vertice.append(position);
            position.x -= 1;
            vertice.append(position);
            vertice.append(position);
            position.z += 1;
            vertice.append(position);
            position.x += 1;
            vertice.append(position);
            uv.append_array(PackedVector2Array(texture_uv.get("down",PackedVector2Array())));
            break;
        case 2:
            position.y += 1;
            vertice.append(position);
            position.z += 1;
            vertice.append(position);
            position.y -= 1;
            vertice.append(position);
            vertice.append(position);
            position.z -= 1;
            vertice.append(position);
            position.y += 1;
            vertice.append(position);
            uv.append_array(PackedVector2Array(texture_uv.get("west",PackedVector2Array())));
            break;
        case 3:
            position += Vector3(1,1,1);
            vertice.append(position);
            position.z -= 1;
            vertice.append(position);
            position.y -= 1;
            vertice.append(position);
            vertice.append(position);
            position.z += 1;
            vertice.append(position);
            position.y += 1;
            vertice.append(position);
            uv.append_array(PackedVector2Array(texture_uv.get("east",PackedVector2Array())));
            break;
        case 4:
            position += Vector3(1,1,0);
            vertice.append(position);
            position.x -= 1;
            vertice.append(position);
            position.y -= 1;
            vertice.append(position);
            vertice.append(position);
            position.x += 1;
            vertice.append(position);
            position.y += 1;
            vertice.append(position);
            uv.append_array(PackedVector2Array(texture_uv.get("north",PackedVector2Array())));
            break;
        case 5:
            position += Vector3(0,1,1);
            vertice.append(position);
            position.x += 1;
            vertice.append(position);
            position.y -= 1;
            vertice.append(position);
            vertice.append(position);
            position.x -= 1;
            vertice.append(position);
            position.y += 1;
            vertice.append(position);
            uv.append_array(PackedVector2Array(texture_uv.get("south",PackedVector2Array())));
            break;
    }
			
}