extends Node
class_name World

#@onready var chunk_pool: ChunkPool = $ChunkPool
var world_info : WorldInfo
var dir : DirAccess = null
var players : Array[Player] = []
var dimension : Dimension = null

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	dimension = Dimension.new()
	dimension.camera = $Player2
	add_child(dimension)
	dimension.players.append($Player2)
	get_tree().current_scene.get_node("TestUI/MainPanel").start.connect(start)

##初始化存档
func first_build_world_info() -> void:
	world_info.create_time = Time.get_unix_time_from_system()
	
func reload() -> void:
	pass

func load_world(path : String) -> void:
	pass

func start(dis : int , level : int) -> void:
	var manager = RenderManager.new()
	add_child(manager)
	manager.set_camera($Player2)
	manager.set_render_distance(dis)
	manager.set_merge_level(level)
	manager.set_chunk_manager(dimension.manager)
	manager.start()
