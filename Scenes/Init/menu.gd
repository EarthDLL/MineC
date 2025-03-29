extends Node

@onready var anima: AnimationPlayer = $Anima

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	anima.play("Cemera")


func button_pressed_start() -> void:
	get_tree().change_scene_to_file("res://Scenes/World/World.tscn")
