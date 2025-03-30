#ifndef ChunkManager_H
#define ChunkManager_H

#include <atomic>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/noise.hpp>
#include <Chunk.h>
#include <RenderTool.h>

using namespace godot;

class ChunkManager : public Node3D{
    GDCLASS(ChunkManager, Node3D);

private:
    static void _bind_methods();
    int render_distance = 6;
    Node3D* camera;
    Array instances;
    Dictionary chunks;
    Thread* load_chunk_thread;
    std::atomic<bool> is_thread_running{false};
    RenderTool* render_tool;
    Ref<Noise> noise;
public:
    void _ready();
    void set_render_distance(int value);
    void set_camera(Node3D* value);
    void start();
    void _physics_process(float _delta);
    Vector2i get_chunk_id_by_pos(Vector3 pos);
    void create_chunk(Vector2i id);
    void create_all_chunks(Array pool);
    void update();
};


#endif