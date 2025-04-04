#include "RenderManager.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void RenderManager :: _bind_methods() {
    ClassDB::bind_method(D_METHOD("set_merge_level","level"), &RenderManager::set_merge_level);
    ClassDB::bind_method(D_METHOD("set_camera","node"), &RenderManager::set_camera);
    ClassDB::bind_method(D_METHOD("set_chunk_manager","manager"), &RenderManager::set_chunk_manager);
    ClassDB::bind_method(D_METHOD("start"), &RenderManager::start);
    ClassDB::bind_method(D_METHOD("render_all_chunks","pool"), &RenderManager::render_all_chunks);
    ClassDB::bind_method(D_METHOD("set_render_distance","count"), &RenderManager::set_render_distance);
}

void RenderManager :: set_camera(Node3D* value){
    camera = value;
}

void RenderManager:: set_merge_level(int value){
    merge_level = value;
}

void RenderManager :: set_render_distance(int value){
    render_distance = value;
}

void RenderManager::start(){
    set_physics_process(true);
}

void RenderManager::_physics_process(float _delta){
    if(Engine::get_singleton() -> get_physics_frames() % 60 == 0 && camera != nullptr){
        update();
    }
}

bool RenderManager::render_chunk(Vector2i chunk_id){
    if(chunk_manager -> has_chunk(chunk_id) == false || data_pool.has(chunk_id) == true){
        return false;
    }
    Ref<Chunk> chunk = chunk_manager -> get_chunk(chunk_id);
    render_tool -> render(chunk);
    ChunkRenderData* render_data_ptr = memnew(ChunkRenderData);
    Ref<ChunkRenderData> render_data = render_data_ptr;
    render_data -> set_main_data(render_tool -> get_render_data());
    render_data -> set_id(chunk_id);
    data_pool.set(chunk_id , render_data);
    edge_pool.append(chunk_id);
    try_render_edge(chunk_id , chunk , render_data);
    
    try_merge(false);
    return true;
}

void RenderManager::test_edge(Ref<ChunkRenderData> data){
    if(data -> is_completed == true){
        edge_pool.erase(data -> chunk_id);
        merge_pool.append(data -> chunk_id);
    }
    return;
}

void RenderManager::try_render_edge(Vector2i chunk_id ,Ref<Chunk> chunk , Ref<ChunkRenderData> self){
    Vector2i goal_id = chunk_id + Vector2i(1,0);
    if(edge_pool.has(goal_id) == true){
        Variant chunk_var = data_pool.get(goal_id,Variant());
        if(chunk_var.get_type() == Variant::OBJECT){
            Ref<ChunkRenderData> goal_chunk_data = Object::cast_to<ChunkRenderData>(chunk_var);
            Ref<Chunk> goal_chunk = chunk_manager -> create_chunk(goal_id);
            if(goal_chunk_data -> has_edge_west == false){
                render_tool -> render_edge(goal_chunk , 2 , chunk);
                goal_chunk_data -> set_west_data(render_tool -> get_render_data());
                test_edge(goal_chunk_data);
            }
            if(self -> has_edge_east == false){
                render_tool -> render_edge(chunk , 3 , goal_chunk);
                self -> set_east_data(render_tool -> get_render_data());
                test_edge(self);
            }
            
        }

    }
    goal_id = chunk_id + Vector2i(-1,0);
    if(edge_pool.has(goal_id) == true){
        Variant chunk_var = data_pool.get(goal_id,Variant());
        if(chunk_var.get_type() == Variant::OBJECT){
            Ref<ChunkRenderData> goal_chunk_data = Object::cast_to<ChunkRenderData>(chunk_var);
            Ref<Chunk> goal_chunk = chunk_manager -> create_chunk(goal_id);
            if(goal_chunk_data -> has_edge_east == false){
                render_tool -> render_edge(goal_chunk , 3 , chunk);
                goal_chunk_data -> set_east_data(render_tool -> get_render_data());
                test_edge(goal_chunk_data);
            }
            if(self -> has_edge_west == false){
                render_tool -> render_edge(chunk , 2 , goal_chunk);
                self -> set_west_data(render_tool -> get_render_data());
                test_edge(self);
            }
        }
    }
    goal_id = chunk_id + Vector2i(0,-1);
    if(edge_pool.has(goal_id) == true){
        Variant chunk_var = data_pool.get(goal_id,Variant());
        if(chunk_var.get_type() == Variant::OBJECT){
            Ref<ChunkRenderData> goal_chunk_data = Object::cast_to<ChunkRenderData>(chunk_var);
            Ref<Chunk> goal_chunk = chunk_manager -> create_chunk(goal_id);
            if(goal_chunk_data -> has_edge_south == false){
                render_tool -> render_edge(goal_chunk , 5 , chunk);
                goal_chunk_data -> set_south_data(render_tool -> get_render_data());
                test_edge(goal_chunk_data);
            }
            if(self -> has_edge_north == false){
                render_tool -> render_edge(chunk , 4 , goal_chunk);
                self -> set_north_data(render_tool -> get_render_data());
                test_edge(self);
            }
        }
    }
    goal_id = chunk_id + Vector2i(0,1);
    if(edge_pool.has(goal_id) == true){
        Variant chunk_var = data_pool.get(goal_id,Variant());
        if(chunk_var.get_type() == Variant::OBJECT){
            Ref<ChunkRenderData> goal_chunk_data = Object::cast_to<ChunkRenderData>(chunk_var);
            Ref<Chunk> goal_chunk = chunk_manager -> create_chunk(goal_id);
            if(goal_chunk_data -> has_edge_north == false){
                render_tool -> render_edge(goal_chunk , 4 , chunk);
                goal_chunk_data -> set_north_data(render_tool -> get_render_data());
                test_edge(goal_chunk_data);
            }
            if(self -> has_edge_south == false){
                render_tool -> render_edge(chunk , 5 , goal_chunk);
                self -> set_south_data(render_tool -> get_render_data());
                test_edge(self);
            }
        }
    }
    return;
}

void RenderManager::add_to_data(Array data , PackedVector3Array& vertice, PackedVector2Array& uv){
    for(int index = 0; index < data.size() ; index++){
        vertice.append_array(PackedVector3Array(Array(data[index])[0]));
        uv.append_array(PackedVector2Array(Array(data[index])[1]));
    }
    return;
}

void RenderManager::try_merge(bool force = false){
    if(force == true){
        for(int i = 0 ; i < edge_pool.size() ; i++){
            merge_pool.append(edge_pool[i]);
        }
        edge_pool = Array();
    }
    while(merge_pool.size() >= merge_level || (force == true && merge_pool.size() > 0)){
        Array array = Array();
        array.resize(Mesh::ARRAY_MAX);
        PackedVector2Array uv;
        PackedVector3Array vertice;
        for(int j = 0; j < merge_level ; j++){
            if(merge_pool.size() == 0){
                break;
            }
            Variant render_var = data_pool.get(merge_pool[0],Variant());
            merge_pool.remove_at(0);
            if(render_var.get_type() == Variant::OBJECT){
                Ref<ChunkRenderData> render_data = Object::cast_to<ChunkRenderData>(render_var);
                Array data = render_data -> main_data;
                add_to_data(data , vertice , uv);
                if(render_data -> has_edge_west){
                    data = render_data -> edge_data_west;
                    add_to_data(data , vertice , uv);
                }
                if(render_data -> has_edge_east){
                    data = render_data -> edge_data_east;
                    add_to_data(data , vertice , uv);
                }
                if(render_data -> has_edge_north){
                    data = render_data -> edge_data_north;
                    add_to_data(data , vertice , uv);
                }
                if(render_data -> has_edge_south){
                    data = render_data -> edge_data_south;
                    add_to_data(data , vertice , uv);
                }
            }
        }
        array[Mesh::ARRAY_VERTEX] = vertice;
	    array[Mesh::ARRAY_TEX_UV] = uv;
        MeshInstance3D* instance = memnew(MeshInstance3D);
        ArrayMesh* mesh = memnew(ArrayMesh);
        mesh -> add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES,array);
        instance -> set_mesh(mesh);
        instance -> set_position(Vector3(0,0,0));
        instance -> set_material_override(render_tool -> ResManager -> get("block_material"));
        call_deferred("add_child",instance);
    }
    return;
}

void RenderManager :: _ready(){
    set_physics_process(false);
    render_tool = memnew(RenderTool);
    render_tool -> init();
    return;
}

void RenderManager::render_all_chunks(Array pool){
    is_thread_running.store(true);
    chunk_manager -> set_lock(true);
    for(int i=0;i<pool.size();i++){
        Variant chunk_id = pool[i];
        Ref<Chunk> chunk = chunk_manager -> create_chunk(Vector2i(chunk_id));
        render_chunk(chunk -> chunk_id);
    }
    try_merge(true);
    is_thread_running.store(false);
    chunk_manager -> set_lock(false);
    Engine::get_singleton()->get_singleton("Debuger")->call("output_time");
    UtilityFunctions::print("Finish");
    return;
}

void RenderManager::set_chunk_manager(ChunkManager* value){
    chunk_manager = value;
}

bool RenderManager::is_chunk_rendered(Vector2i chunk_id){
    return data_pool.has(chunk_id);
}

void RenderManager::update(){
    if(is_thread_running.load() == true){
        return;
    }else{
            if(load_chunk_thread != nullptr){
            load_chunk_thread -> wait_to_finish();
            load_chunk_thread = Ref<Thread>();
        }
    }

    Vector2i current_chunk_id = chunk_manager -> get_chunk_id_by_pos(camera -> get_global_position());
    Array wait_pool;
    for(int x = current_chunk_id.x - render_distance ; x < current_chunk_id.x+ render_distance +1;x++){
        for(int y = current_chunk_id.y-render_distance; y < current_chunk_id.y+render_distance+1;y++){
            Vector2i id = Vector2i(x,y);
            if(data_pool.has(id) == false){
                wait_pool.append(id);
            }
        }
    }
    if(wait_pool.size() > 0){
        load_chunk_thread = Ref<Thread>(memnew(Thread));
        Engine::get_singleton()->get_singleton("Debuger")->call("output_time");
        Callable callable = Callable(this , "render_all_chunks").bind(wait_pool);
        load_chunk_thread -> start(callable);
    }
}
