#ifndef BlockModelInfo_H
#define BlockModelInfo_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class BlockModelInfo : public Resource{
    GDCLASS(BlockModelInfo, Resource);

private:
    String type_id = "";
public:
    enum CubeSide{
        UP = 0,
        DOWN = 1,
        WEST = 2,
        EAST = 3,
        NORTH = 4,
        SOUTH = 5
    };
    enum DealType { 
        None = 0,
        FacingOnly = 1,
        DirectionOnly = 2,
        Both = 3,
        Custom = 4
    };

    static void _bind_methods();

    bool is_cube = true;
    int deal_type = 0;
    String custom_model = "";
    Dictionary texture_pieces;

    Dictionary texture_uv;

    void set_cube_block(bool value);
    bool is_cube_block();
    void set_type_id(String value);
    String get_type_id();
    void set_custom_model(String value);
    String get_custom_model();
    void set_deal_type(int value);
    int get_deal_type();
    void set_texture_pieces(Dictionary value);
    Dictionary get_texture_pieces();
    Array translate_data();
    void deal_uv(Dictionary uv_base);
};

VARIANT_ENUM_CAST(BlockModelInfo :: CubeSide);
VARIANT_ENUM_CAST(BlockModelInfo :: DealType);

#endif