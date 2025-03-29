extends Node3D
class_name ChunkManager

var range := 16
var camera : Node3D = null
var render_table := {}
var instances : Array[MeshInstance3D] = []
var chunks := {}
var load_chunk_thread := Thread.new()
var render_tool : RenderTool = RenderTool.new();

func start() -> void:
	range = roundf(get_tree().current_scene.get_node("TestUI/MainPanel/MainList/RenderDis/RenderDis").value)
	set_physics_process(true)

func _ready() -> void:
	set_physics_process(false)
	get_tree().current_scene.get_node("TestUI/MainPanel").start.connect(start)
	render_tool.init()
	return

func _physics_process(_delta: float) -> void:
	if Engine.get_physics_frames() % 60 == 0 && is_instance_valid(camera):
		update()
		
func get_chunk_id_by_pos(pos : Vector3) -> Vector2i:
	return Vector2i((Vector2(pos.x,pos.z)/16).floor())
const noise = preload("res://Resources/Noise/Noise.tres")
func create_chunk(id : Vector2i) -> void:
	#var chunk := Chunk.new(id)
	var chunk := Chunk.new()
	chunk.init(id , Vector3i(16,256,16))
	chunk.load_by_noise(noise)
	chunks[id] = chunk
	render_tool.render(chunk)
	var instance := MeshInstance3D.new()
	var mesh := ArrayMesh.new()
	mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES,render_tool.get_render_data())
	instance.mesh = mesh
	instance.position = Vector3(id.x * 16, 0 , id.y* 16)
	instance.material_override = ResManager.block_material
	call_deferred("add_child",instance)
	
func create_all_chunks(arr : Array) -> void:
	for id in arr:
		create_chunk(id)
	Debuger.call_deferred("output_time")

func update() -> void:
	if load_chunk_thread.is_started():
		if load_chunk_thread.is_alive():
			return
		else:
			load_chunk_thread.wait_to_finish()
	
	load_chunk_thread = Thread.new()
	var chunk_id := get_chunk_id_by_pos(camera.global_position)
	var chunk_wait_pool := []
	for x : int in range(chunk_id.x-range,chunk_id.x+range+1):
		for y : int in range(chunk_id.y-range,chunk_id.y+range+1):
			var id  := Vector2i(x,y)
			if !chunks.has(id):
				chunk_wait_pool.append(id)
	if chunk_wait_pool.size() > 0:
		Debuger.output_time()
		load_chunk_thread.start(Callable(self,"create_all_chunks").bind(chunk_wait_pool))
