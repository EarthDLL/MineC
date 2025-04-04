#ifndef ChunkRenderData_H
#define ChunkRenderData_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class ChunkRenderData : public RefCounted{
    GDCLASS(ChunkRenderData, RefCounted);

private:
    static void _bind_methods();
public:
    bool is_completed = false;

    Vector2i chunk_id;
    bool has_edge_north = false;
    bool has_edge_south = false;
    bool has_edge_west = false;
    bool has_edge_east = false;
    
    Array main_data;
    Array edge_data_north;
    Array edge_data_south;
    Array edge_data_west;
    Array edge_data_east;
    Array other_data;
    
    void set_main_data(Array data);
    void set_north_data(Array data);
    void set_south_data(Array data);
    void set_west_data(Array data);
    void set_east_data(Array data);
    void set_other_data(Array data);
    void set_id(Vector2i id);
    
    
};


#endif