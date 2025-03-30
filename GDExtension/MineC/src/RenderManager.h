#ifndef RenderManager_H
#define RenderManager_H

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
#include <ChunkManager.h>
#include <RenderTool.h>

using namespace godot;

class RenderManager : public Node3D{
    GDCLASS(RenderManager, Node3D);

private:
    static void _bind_methods();
    int render_distance = 6;
    Node3D* camera;
    Array instances;
    Ref<Thread> load_chunk_thread;
    std::atomic<bool> is_thread_running{false};
    RenderTool* render_tool;
    ChunkManager* chunk_manager;
public:
    void _ready();
    void set_render_distance(int value);
    void set_camera(Node3D* value);
    void set_chunk_manager(ChunkManager* value);
    void start();
    void _physics_process(float _delta);
    void render_chunk(Ref<Chunk> chunk);
    void render_all_chunks(Array pool);
    void update();
};


#endif