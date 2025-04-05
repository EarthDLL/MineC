#include "RenderManager.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
//
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

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
    data_pool.set(chunk_id , 0);
    edge_pool.set(chunk_id , render_data);
    try_render_edge(chunk_id , chunk , render_data);
    
    try_merge(false,false);
    return true;
}

void RenderManager::test_edge(Ref<ChunkRenderData> data){
    if(data -> is_completed == true){
        if(int(data_pool.get(data -> chunk_id,0)) == 2){
            break_chunk_mesh(data -> chunk_id);
        }
        merge_pool.set(data -> chunk_id , data);
        edge_pool.erase(data -> chunk_id);
        data_pool.set(data -> chunk_id , 1);
    }
    return;
}

void RenderManager::try_render_edge(Vector2i chunk_id ,Ref<Chunk> chunk , Ref<ChunkRenderData> self){
    Vector2i goal_id = chunk_id + Vector2i(1,0);
    if(edge_pool.has(goal_id) == true){
        Variant chunk_var = edge_pool.get(goal_id,Variant());
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
        Variant chunk_var = edge_pool.get(goal_id,Variant());
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
        Variant chunk_var = edge_pool.get(goal_id,Variant());
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
        Variant chunk_var = edge_pool.get(goal_id,Variant());
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

void RenderManager::break_chunk_mesh(Vector2i chunk_id){
   //将合并的Mesh包含的Chunk全部清空
   if(data_pool.has(chunk_id) == false || mesh_pool.has(chunk_id) == false){
    return;
   }
   Array chunks = mesh_pool.get(chunk_id,Variant());
   Variant mesh_var = chunks[0];
   chunks.remove_at(0);
   if(mesh_var.get_type() == Variant::OBJECT){
    MeshInstance3D* mesh = Object::cast_to<MeshInstance3D>(mesh_var);
        for(int i = 0; i < chunks.size();i++){
            mesh_pool.erase(chunks[i]);
        }
        switch(int(data_pool.get(chunk_id , 0))){
            case 2:
                for(int i = 0; i < chunks.size();i++){
                    if(is_chunk_out(camera_pos,Vector2i(chunks[i]))){
                        edge_pool.erase(chunks[i]);
                        data_pool.erase(chunks[i]);
                        cache_pool.erase(chunks[i]);
                    }else{
                        data_pool.set(chunks[i],0);
                    }
                }
            break;
            case 3:
                for(int i = 0; i < chunks.size();i++){
                    if(is_chunk_out(camera_pos,Vector2i(chunks[i]))){
                        data_pool.erase(chunks[i]);
                        cache_pool.erase(chunks[i]);
                    }else{
                        data_pool.set(chunks[i],1);
                        merge_pool.set(chunks[i],cache_pool.get(chunks[i],nullptr));
                    }
                }
            break;
            case 4:
                for(int i = 0; i < chunks.size();i++){
                    data_pool.erase(chunks[i]);
                }
        }
        mesh -> queue_free();
    }
}

void RenderManager::try_merge(bool force = false ,bool use_unable_completed = false){
    if(use_unable_completed == true){
        Array keys = edge_pool.keys();
        for(int i = 0 ; i < keys.size() ; i++){
            if(int(data_pool.get(keys[i],2)) == 0){
                merge_pool.set(keys[i],edge_pool.get(keys[i],nullptr));
            }
        }
    }

    while(merge_pool.size() >= merge_level || (force == true && merge_pool.size() > 0)){
        Array consist_chunks;
        Array array = Array();
        array.resize(Mesh::ARRAY_MAX);
        PackedVector2Array uv;
        PackedVector3Array vertice;
        MeshInstance3D* instance = memnew(MeshInstance3D);
        consist_chunks.append(instance);
        for(int j = 0; j < merge_level ; j++){
            if(merge_pool.size() == 0){
                break;
            }
            Variant key = merge_pool.keys()[0]; //key就是chunk的id
            Variant render_var = merge_pool.get(key,nullptr);
            merge_pool.erase(key);
            consist_chunks.append(key);
            mesh_pool.set(key,consist_chunks);

            if(render_var.get_type() == Variant::OBJECT){
                Ref<ChunkRenderData> render_data = Object::cast_to<ChunkRenderData>(render_var);
                //先改状态
                if(use_unable_completed == true){
                    data_pool.set(key , 2);
                }else{
                    cache_pool.set(key , render_data);
                    data_pool.set(key , 3);
                }

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
        ArrayMesh* mesh = memnew(ArrayMesh);
        mesh -> add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES,array);
        instance -> set_mesh(mesh);
        instance -> set_position(Vector3(0,0,0));
        instance -> set_material_override(render_tool -> ResManager -> get("block_material"));
        call_deferred("add_child",instance);
        /*StaticBody3D* body = memnew(StaticBody3D);
        body -> set_position(Vector3(0,0,0));
        CollisionShape3D* coll = memnew(CollisionShape3D);
        Ref<ConcavePolygonShape3D> shape = mesh -> create_trimesh_shape();
        coll -> set_shape(shape);
        body -> add_child(coll);
        call_deferred("add_child",body);*/
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
    try_merge(true,false);
    try_merge(true,true);
    is_thread_running.store(false);
    chunk_manager -> set_lock(false);
    Engine::get_singleton()->get_singleton("Debuger")->call("output_time");
    UtilityFunctions::print("Finish");
    return;
}

void RenderManager::set_chunk_manager(ChunkManager* value){
    chunk_manager = value;
}

bool RenderManager::is_chunk_out(Vector2 camera_pos , Vector2i chunk_id){
    int physic_distance = chunk_manager -> physic_distance;
    if(camera_pos.distance_squared_to(Vector2(chunk_id.x * 16 + 8 , chunk_id.y * 16 + 8)) > 256 * (physic_distance+1) * (physic_distance+1)){
        return true;
    }
    return false;
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
    //删除过远区块
    camera_pos = Vector2(camera -> get_global_position().x , camera -> get_global_position().z);
    Array keys = data_pool.keys();
    for(int i = 0; i < keys.size() ; i++){
        int state = data_pool.get(keys[i],Vector2i());
        if(state == 2 || state == 3 || state ==4){
            if(is_chunk_out(camera_pos , Vector2(keys[i]))){
                break_chunk_mesh(Vector2(keys[i]));
            }
        }
    }

    //检测新区块
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
