#include "Chunk.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
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