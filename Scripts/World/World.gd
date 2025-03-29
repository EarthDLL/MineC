extends Node
class_name World

#@onready var chunk_pool: ChunkPool = $ChunkPool
var world_info : WorldInfo
var dir : DirAccess = null
var players : Array[Player] = []

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var dimension := Dimension.new()
	dimension.camera = $Player2
	add_child(dimension)
	dimension.players.append($Player2)

##初始化存档
func first_build_world_info() -> void:
	world_info.create_time = Time.get_unix_time_from_system()
	
func reload() -> void:
	pass

func load_world(path : String) -> void:
	pass
