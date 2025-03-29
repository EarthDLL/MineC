#include "BlockInfo.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

void BlockInfo ::_bind_methods() {
    ClassDB::bind_method(D_METHOD("translate_data"), &BlockInfo::translate_data);
    ClassDB::bind_method(D_METHOD("deal_uv"), &BlockInfo::deal_uv);
    ClassDB::bind_method(D_METHOD("get_type_id"), &BlockInfo::get_type_id);
    ClassDB::bind_method(D_METHOD("set_type_id"), &BlockInfo::set_type_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "type_id") , "set_type_id" , "get_type_id");
    ClassDB::bind_method(D_METHOD("get_id"), &BlockInfo::get_id);
    ClassDB::bind_method(D_METHOD("set_id"), &BlockInfo::set_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "id") , "set_id" , "get_id");
    ClassDB::bind_method(D_METHOD("is_cube_block"), &BlockInfo::is_cube_block);
    ClassDB::bind_method(D_METHOD("set_cube_block"), &BlockInfo::set_cube_block);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cube_block") , "set_cube_block" , "is_cube_block");

    ClassDB::bind_method(D_METHOD("get_custom_geometry"), &BlockInfo::get_custom_geometry);
    ClassDB::bind_method(D_METHOD("set_custom_geometry"), &BlockInfo::set_custom_geometry);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "custom_geometry") , "set_custom_geometry" , "get_custom_geometry");
    ClassDB::bind_method(D_METHOD("get_texture_type"), &BlockInfo::get_texture_type);
    ClassDB::bind_method(D_METHOD("set_texture_type"), &BlockInfo::set_texture_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_type" , PROPERTY_HINT_ENUM, "Normal , Custom") , "set_texture_type" , "get_texture_type");
    ClassDB::bind_method(D_METHOD("get_texture_pieces"), &BlockInfo::get_texture_pieces);
    ClassDB::bind_method(D_METHOD("set_texture_pieces"), &BlockInfo::set_texture_pieces);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "texture_pieces" , PROPERTY_HINT_DICTIONARY_TYPE , "String;String") , "set_texture_pieces" , "get_texture_pieces");
    
    ClassDB::bind_integer_constant("BlockInfo" , "CubeSide" , "UP" , UP);
    ClassDB::bind_integer_constant("BlockInfo" , "CubeSide" , "DOWN" , DOWN);
    ClassDB::bind_integer_constant("BlockInfo" , "CubeSide" , "WEST" , WEST);
    ClassDB::bind_integer_constant("BlockInfo" , "CubeSide" , "EAST" , EAST);
    ClassDB::bind_integer_constant("BlockInfo" , "CubeSide" , "NORTH" , NORTH);
    ClassDB::bind_integer_constant("BlockInfo" , "CubeSide" , "SOUTH" , SOUTH);
}

void BlockInfo::set_cube_block(bool value){
    cube_block = value;
    return;
}

bool BlockInfo::is_cube_block(){
    return cube_block;
}

void BlockInfo::set_id(int value){
    if(value >= 0){
        id = value;
    }
    return;
}

int BlockInfo::get_id(){
    return id;
}

void BlockInfo::set_type_id(String value){
    type_id = value;
    return;
}
String BlockInfo::get_type_id(){
    return type_id;
}

void BlockInfo::set_custom_geometry(String value){
    custom_geometry = value;
    return;
}
String BlockInfo::get_custom_geometry(){
    return custom_geometry;
}
void BlockInfo::set_texture_type(int value){
    if(value >= 0 && value <2){
        texture_type = value;
    }
    return;
}
int BlockInfo::get_texture_type(){
    return texture_type;
}
void BlockInfo::set_texture_pieces(Dictionary value){
    texture_pieces = value;
    return;
}
Dictionary BlockInfo::get_texture_pieces(){
    return texture_pieces;
}

Array BlockInfo::translate_data(){
    return Array();
}

void BlockInfo::deal_uv(Dictionary uv_base , Array faces){
    if(cube_block == false){
        return;
    }
    PackedVector2Array default_uv = uv_base.get("miss",PackedVector2Array());
    if(texture_pieces.has("*") && uv_base.has(texture_pieces.get("*",PackedVector2Array()))){
        default_uv = uv_base.get(texture_pieces.get("*",PackedVector2Array()),PackedVector2Array());
    }
    for(int i = 0 ; i < faces.size(); i++){
        String face_name = faces[i];
        texture_uv.set(face_name , uv_base.get(texture_pieces.get(face_name,nullptr),default_uv));
    }
}

/*
func translate_data() -> Array:
	var data := []
	return []
	
func deal_uv() -> void:
	if is_cube:
		var default : PackedVector2Array = ResManager.block_texture_uvs.miss
		if texture_pieces.has("*") && ResManager.block_texture_uvs.has(texture_pieces["*"]):
			default = ResManager.block_texture_uvs[texture_pieces["*"]]
		for piece : String in ResManager.cube_geo.textures:
			if texture_pieces.has(piece) && ResManager.block_texture_uvs.has(piece):
				texture_uv[piece] = ResManager.block_texture_uvs[piece]
			else:
				texture_uv[piece] = default
	texture_pieces = {}
*/