extends CharacterBody3D

#@onready var touch_joy: TouchJoy = $TouchJoy
#
#func _physics_process(delta: float) -> void:
#	velocity = touch_joy.get_movement() * 10
#	move_and_slide()
var dimension : Dimension = null

func _ready() -> void:
	if get_parent() is Dimension:
		dimension = get_parent()
	add_to_group("Players")

func get_position_chunk() -> Vector2i:
	return Vector2i(floori(global_position.x/16),floori(global_position.y/16))

func get_dimension() -> String:
	if dimension == null:
		return ""
	return dimension.id
