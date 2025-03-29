extends Node3D
class_name Dimension

@export var camera : Node3D = null
@onready var manager : ChunkManager = ChunkManager.new()

var id : String = "minecraft:overworld"
var world : World = null
var players : Array[Player] = []


func _ready() -> void:
	world = get_parent()
	manager.camera = camera
	add_child(manager)
