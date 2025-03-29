#ifndef BlockInfo_H
#define BlockInfo_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class BlockInfo : public Resource{
    GDCLASS(BlockInfo, Resource);

private:
    int id = 0;
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
    enum TextureType { 
        Normal = 0,
        Custom = 1
    };

    static void _bind_methods();
    bool cube_block = true;
    String custom_geometry = "";
    int texture_type = 0;
    Dictionary texture_pieces;
    Dictionary texture_uv;

    void set_cube_block(bool value);
    bool is_cube_block();
    void set_id(int value);
    int get_id();
    void set_type_id(String value);
    String get_type_id();
    void set_custom_geometry(String value);
    String get_custom_geometry();
    void set_texture_type(int value);
    int get_texture_type();
    void set_texture_pieces(Dictionary value);
    Dictionary get_texture_pieces();
    Array translate_data();
    void deal_uv(Dictionary uv_base, Array faces);
};

VARIANT_ENUM_CAST(BlockInfo :: CubeSide);
VARIANT_ENUM_CAST(BlockInfo :: TextureType);

#endif