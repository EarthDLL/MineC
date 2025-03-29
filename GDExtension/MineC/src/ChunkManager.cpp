#include "ChunkManager.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

void ChunkManager :: _bind_methods() {
    ClassDB::bind_method(D_METHOD("set_camera"), &ChunkManager::set_camera);
    ClassDB::bind_method(D_METHOD("set_render_distance"), &ChunkManager::set_render_distance);
}

void ChunkManager :: set_camera(Node3D* value){
    camera = value;
}

void ChunkManager :: set_render_distance(int value){
    render_distance = value;
}

void ChunkManager::start(){
    set_physics_process(true);
}

void ChunkManager::_physics_process(float _delta){
    if(Engine::get_singleton() -> get_physics_frames() % 60 == 0 && camera != nullptr){
        update();
    }
}

Vector2i ChunkManager::get_chunk_id_by_pos(Vector3 pos){
    return Vector2i((Vector2(pos.x,pos.z)/16).floor());
}

void ChunkManager::create_chunk(Vector2i id){
    Chunk* chunk = memnew(Chunk);
    chunk -> init(id , Vector3i(16,256,16));
    chunk -> load_by_noise(noise);
    chunks.set(id , chunk);
    render_tool -> render(chunk);
    MeshInstance3D* instance = memnew(MeshInstance3D);
    ArrayMesh* mesh = memnew(ArrayMesh);
    mesh -> add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES,render_tool -> get_render_data());
    instance -> set_mesh(mesh);
	instance -> set_position(Vector3(id.x * 16, 0 , id.y* 16));
	instance -> set_material_override(render_tool -> ResManager -> get("block_material"));
    call_deferred("add_child",instance);
}

void ChunkManager :: _ready(){
    noise = ResourceLoader::get_singleton()->load("res://Resources/Noise/Noise.tres");
    set_physics_process(false);
    render_tool = memnew(RenderTool);
    render_tool -> init();
    return;
}

void ChunkManager::create_all_chunks(Array pool){
    for(int i=0;i<pool.size();i++){
        Variant chunk_id = pool[i];
        create_chunk(Vector2i(chunk_id));
    }
    Engine::get_singleton()->get_singleton("Debuger")->call("output_time");
}

void ChunkManager::update(){
    if(load_chunk_thread != nullptr && load_chunk_thread -> is_started()){
        if(load_chunk_thread -> is_alive()){
            return;
        }else{
            load_chunk_thread -> wait_to_finish();
        }
    }

    load_chunk_thread = memnew(Thread);
    Vector2i current_chunk_id = get_chunk_id_by_pos(camera -> global_position);
    Array wait_pool;
    for(int x = current_chunk_id.x - render_distance ; x < current_chunk_id.x+ render_distance +1;x++){
        for(int y = current_chunk_id.y-render_distance; y < current_chunk_id.y+render_distance+1;y++){
            Vector2i id = Vector2i(x,y);
            if(chunks.has(id) == false){
                wait_pool.append(id);
            }
        }
    }
    if(wait_pool.size() > 0){
        Engine::get_singleton()->get_singleton("Debuger")->call("output_time");
        Callable callable = Callable(create_all_chunks).bind(wait_pool);
        load_chunk_thread -> start(callable);
    }
}
