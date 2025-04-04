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
    data_pool.set(chunk_id , render_data);
    edge_pool.append(render_data);
    try_merge();
    
    MeshInstance3D* instance = memnew(MeshInstance3D);
    ArrayMesh* mesh = memnew(ArrayMesh);
    mesh -> add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES,render_tool -> get_render_data());
    instance -> set_mesh(mesh);
	instance -> set_position(Vector3(chunk -> chunk_id.x * 16, 0 ,chunk -> chunk_id.y* 16));
	instance -> set_material_override(render_tool -> ResManager -> get("block_material"));
    call_deferred("add_child",instance);
    return true;
}

void RenderManager::try_merge(){
    
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
    is_thread_running.store(false);
    chunk_manager -> set_lock(false);
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
            if(chunk_manager -> has_chunk(id) == false){
                wait_pool.append(id);
                //这里逻辑错误，应该查RenderDataPool，不要查chunk_manager，不然chunk_manager一直卸载，这里又会循环加载
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
