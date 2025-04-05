#ifndef ChunkManager_H
#define ChunkManager_H

#include <atomic>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/noise.hpp>
#include <Chunk.h>
#include <RenderTool.h>

using namespace godot;

class ChunkManager : public Node3D{
    GDCLASS(ChunkManager, Node3D);

private:
    static void _bind_methods();
    Node3D* camera;
    Dictionary chunks;
    Ref<Noise> noise;
    std::atomic<bool> is_thread_running{false};
public:
    float physic_distance = 6;
    void _ready();
    void set_camera(Node3D* value);
    void _physics_process(float _delta);
    Vector2i get_chunk_id_by_pos(Vector3 pos);
    Ref<Chunk> create_chunk(Vector2i id);
    void create_all_chunks(Array pool);
    void update();
    void set_lock(bool value);
    Ref<Chunk> get_chunk(Vector2i id);
    bool has_chunk(Vector2i id);
    bool is_locked = false;
};


#endif