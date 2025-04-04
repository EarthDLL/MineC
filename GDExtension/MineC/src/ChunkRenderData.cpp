#include "ChunkRenderData.h"

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;


void ChunkRenderData::_bind_methods() {
    return;
}

void ChunkRenderData ::update(){
    if(has_edge_north == true && has_edge_south == true && has_edge_west == true && has_edge_east == true){
        is_completed = true;
    }
    else{
        is_completed = false;
    }
    return;
}

void ChunkRenderData::set_main_data(Array data){
    main_data = data;
}

void ChunkRenderData::set_south_data(Array data){
    edge_data_south = data;
    has_edge_south = true;
    update();
}

void ChunkRenderData::set_west_data(Array data){
    edge_data_west = data;
    has_edge_west = true;
    update();
}

void ChunkRenderData::set_east_data(Array data){
    edge_data_east = data;
    has_edge_east = true;
    update();
}

void ChunkRenderData::set_north_data(Array data){
    edge_data_north = data;
    has_edge_north = true;
    update();
}

void ChunkRenderData::set_other_data(Array data){
    other_data = data;
}

void ChunkRenderData::set_id(Vector2i id){
    chunk_id = id;
}
