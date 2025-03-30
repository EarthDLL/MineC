extends Node3D
class_name Dimension

@export var camera : Node3D = null
@onready var manager : ChunkManager = ChunkManager.new()

var id : String = "minecraft:overworld"
var world : World = null
var players : Array[Player] = []


func _ready() -> void:
	world = get_parent()
	manager.set_camera(camera)
	add_child(manager)
	get_tree().current_scene.get_node("TestUI/MainPanel").start.connect(start)
	
func start(dis : int) -> void:
	manager.set_render_distance(dis)
	manager.start()
	print("ssart")
