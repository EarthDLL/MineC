#include "BlockModelInfo.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

void BlockModelInfo ::_bind_methods() {
    ClassDB::bind_method(D_METHOD("translate_data"), &BlockModelInfo::translate_data);
    ClassDB::bind_method(D_METHOD("deal_uv"), &BlockModelInfo::deal_uv);
    ClassDB::bind_method(D_METHOD("get_type_id"), &BlockModelInfo::get_type_id);
    ClassDB::bind_method(D_METHOD("set_type_id"), &BlockModelInfo::set_type_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "type_id") , "set_type_id" , "get_type_id");
    ClassDB::bind_method(D_METHOD("is_cube_block"), &BlockModelInfo::is_cube_block);
    ClassDB::bind_method(D_METHOD("set_cube_block"), &BlockModelInfo::set_cube_block);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_cube") , "set_cube_block" , "is_cube_block");

    ClassDB::bind_method(D_METHOD("get_custom_model"), &BlockModelInfo::get_custom_model);
    ClassDB::bind_method(D_METHOD("set_custom_model"), &BlockModelInfo::set_custom_model);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "custom_model") , "set_custom_model" , "get_custom_model");
    ClassDB::bind_method(D_METHOD("get_deal_type"), &BlockModelInfo::get_deal_type);
    ClassDB::bind_method(D_METHOD("set_deal_type"), &BlockModelInfo::set_deal_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "deal_type" , PROPERTY_HINT_ENUM, "None , FacingOnly , DirectionOnly , Both , Custom") , "set_deal_type" , "get_deal_type");
    ClassDB::bind_method(D_METHOD("get_texture_pieces"), &BlockModelInfo::get_texture_pieces);
    ClassDB::bind_method(D_METHOD("set_texture_pieces"), &BlockModelInfo::set_texture_pieces);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "texture_pieces" , PROPERTY_HINT_DICTIONARY_TYPE , "String;String") , "set_texture_pieces" , "get_texture_pieces");
    
    ClassDB::bind_integer_constant("BlockModelInfo" , "CubeSide" , "UP" , UP);
    ClassDB::bind_integer_constant("BlockModelInfo" , "CubeSide" , "DOWN" , DOWN);
    ClassDB::bind_integer_constant("BlockModelInfo" , "CubeSide" , "WEST" , WEST);
    ClassDB::bind_integer_constant("BlockModelInfo" , "CubeSide" , "EAST" , EAST);
    ClassDB::bind_integer_constant("BlockModelInfo" , "CubeSide" , "NORTH" , NORTH);
    ClassDB::bind_integer_constant("BlockModelInfo" , "CubeSide" , "SOUTH" , SOUTH);
}

void BlockModelInfo::set_cube_block(bool value){
    is_cube = value;
    return;
}

bool BlockModelInfo::is_cube_block(){
    return is_cube;
}

void BlockModelInfo::set_type_id(String value){
    type_id = value;
    return;
}
String BlockModelInfo::get_type_id(){
    return type_id;
}

void BlockModelInfo::set_custom_model(String value){
    custom_model = value;
    return;
}
String BlockModelInfo::get_custom_model(){
    return custom_model;
}
void BlockModelInfo::set_deal_type(int value){
    if(value >= 0 && value < 4){
        deal_type = value;
    }
    return;
}
int BlockModelInfo::get_deal_type(){
    return deal_type;
}
void BlockModelInfo::set_texture_pieces(Dictionary value){
    texture_pieces = value;
    return;
}
Dictionary BlockModelInfo::get_texture_pieces(){
    return texture_pieces;
}

Array BlockModelInfo::translate_data(){
    return Array();
}

void BlockModelInfo::deal_uv(Dictionary uv_base){
    if(is_cube == false){
        return;
    }
    PackedVector2Array default_uv = uv_base.get("miss",PackedVector2Array());
    if(texture_pieces.has("*") && uv_base.has(texture_pieces.get("*",PackedVector2Array()))){
        default_uv = uv_base.get(texture_pieces.get("*",PackedVector2Array()),PackedVector2Array());
    }
    texture_uv.set("up" , uv_base.get(texture_pieces.get("up",nullptr),default_uv));
    texture_uv.set("down" , uv_base.get(texture_pieces.get("down",nullptr),default_uv));
    texture_uv.set("west" , uv_base.get(texture_pieces.get("west",nullptr),default_uv));
    texture_uv.set("east" , uv_base.get(texture_pieces.get("east",nullptr),default_uv));
    texture_uv.set("south" , uv_base.get(texture_pieces.get("south",nullptr),default_uv));
    texture_uv.set("north" , uv_base.get(texture_pieces.get("north",nullptr),default_uv));
}