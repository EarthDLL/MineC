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
#include <ChunkRenderData.h>

using namespace godot;

class RenderManager : public Node3D{
    GDCLASS(RenderManager, Node3D);

private:
    static void _bind_methods();
    int render_distance = 6;
    int merge_level = 2;
    Node3D* camera;
    bool require_merge;
    Array instances;
    Dictionary data_pool;
    /* 格式：ChunkID : type(int)
    TYPE:
    0 - ChunkRenderData不完整 且 没有合并
    1- ChunkRenderData完整 且 没有合并
    2 - ChunkRenderData不完整已经合并
    3 - ChunkRenderData完整已合并 有缓存
    4 - ChunkRenderData完整并且无缓存
    */
    Dictionary mesh_pool;
    Ref<Thread> load_chunk_thread;
    std::atomic<bool> is_thread_running{false};
    RenderTool* render_tool;
    ChunkManager* chunk_manager;
    
    Dictionary edge_pool; //等待渲染边缘的区块
    Dictionary merge_pool; //要合并的区块池
    Dictionary cache_pool;//缓存的ChunkRenderData
    Vector2 camera_pos;
public:
    void try_render_edge(Vector2i chunk_id ,Ref<Chunk> chunk , Ref<ChunkRenderData> self);
    void _ready();
    void set_merge_level(int value);
    void set_render_distance(int value);
    void set_camera(Node3D* value);
    void set_chunk_manager(ChunkManager* value);
    void start();
    void _physics_process(float _delta);
    bool render_chunk(Vector2i chunk_id);
    void render_all_chunks(Array pool);
    void update();
    void break_chunk_mesh(Vector2i chunk_id);
    void try_merge(bool force,bool use_unable_completed);
    bool is_chunk_out(Vector2i chunk_id , bool use_physic);
    void test_edge(Ref<ChunkRenderData> data);
    void add_to_data(Array data , PackedVector3Array& vertice, PackedVector2Array& uv);
    void break_block_mesh(Vector2i chunk_id);
};


#endif