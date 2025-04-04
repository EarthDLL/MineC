#include "ChunkRenderData.h"

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;


void ChunkRenderData::_bind_methods() {
    return;
}

void ChunkRenderData::set_main_data(Array data){
    main_data = data;
}

void ChunkRenderData::set_south_data(Array data){
    edge_data_south = data;
}

void ChunkRenderData::set_west_data(Array data){
    edge_west_north = data;
}

void ChunkRenderData::set_east_data(Array data){
    edge_data_east = data;
}

void ChunkRenderData::set_north_data(Array data){
    edge_data_north = data;
}

void ChunkRenderData::set_other_data(Array data){
    other_data = data;
}

void ChunkRenderData::set_id(Vector2i id){
    chunk_id = id;
}
