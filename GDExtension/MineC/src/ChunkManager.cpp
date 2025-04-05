#include "ChunkManager.h"

#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void ChunkManager :: _bind_methods() {
    ClassDB::bind_method(D_METHOD("set_camera","node"), &ChunkManager::set_camera);
    ClassDB::bind_method(D_METHOD("create_all_chunks","pool"), &ChunkManager::create_all_chunks);
}

void ChunkManager :: set_camera(Node3D* value){
    camera = value;
}

void ChunkManager::_physics_process(float _delta){
    if(Engine::get_singleton() -> get_physics_frames() % 60 == 0 && camera != nullptr){
        update();
    }
}



Vector2i ChunkManager::get_chunk_id_by_pos(Vector3 pos){
    return Vector2i((Vector2(pos.x,pos.z)/16).floor());
}

Ref<Chunk> ChunkManager::create_chunk(Vector2i id){
    if(has_chunk(id)){
        return get_chunk(id);
    }
    Ref<Chunk> chunk = Ref<Chunk>(memnew(Chunk));
    chunk -> init(id , 0 , 256);
    chunk -> load_by_noise(noise);
    chunks.set(id , chunk);
    return chunk;
}

Ref<Chunk> ChunkManager::get_chunk(Vector2i id){
    Variant value = chunks.get(id ,nullptr);
    return Ref<Chunk>(value);
}

void ChunkManager :: _ready(){
    Ref<Resource> noise_res = ResourceLoader::get_singleton()->load("res://Resources/Noise/Noise.tres");
    noise = Ref<Noise>(Object::cast_to<Noise>(noise_res.ptr()));
    return;
}

void ChunkManager::create_all_chunks(Array pool){
    is_thread_running.store(true);
    for(int i=0;i<pool.size();i++){
        Variant chunk_id = pool[i];
        create_chunk(Vector2i(chunk_id));
    }
    is_thread_running.store(false);
}

bool ChunkManager::has_chunk(Vector2i id){
    return chunks.has(id);
}

void ChunkManager::set_lock(bool value){
    is_locked = value;
}

void ChunkManager::update(){
    if(is_locked){
        return;
    }

    Array keys = chunks.keys();
    Vector2 camera_pos = Vector2(camera -> get_global_position().x , camera -> get_global_position().z);
    for(int i = 0; i < keys.size() ; i++){
        Variant value = chunks.get(keys[i],nullptr);
        Ref<Chunk> chunk = Ref<Chunk>(value);
        if(chunk != nullptr){
            if(camera_pos.distance_squared_to(chunk -> get_center()) > 256 * (physic_distance+1) * (physic_distance+1)){
                chunks.erase(keys[i]);
                chunk -> destroy_chunk();
                chunk.unref();
            }
        }
    }
}
