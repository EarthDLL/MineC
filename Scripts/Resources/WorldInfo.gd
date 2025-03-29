extends Resource
class_name WorldInfo

@export var world_name : String = ""
@export var seed : int = 0
@export var game_mode : int = 0
@export var ticks : int = 0
@export var create_time : int = 0
@export var first_build : bool = false
#@export var chunk_size : int = Chunk.ChunkSize.OVERWORLD_OLD

func _init() -> void:
	pass
