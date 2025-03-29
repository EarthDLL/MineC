extends Node

signal WorldLoad

@export var viewport_size : Vector2i = Vector2.ZERO

@onready var click: AudioStreamPlayer = $Click

func _enter_tree() -> void:
	reload_viewport_size()
	get_tree().get_root().size_changed.connect(reload_viewport_size)
	
func reload_viewport_size() -> void:
	#window_size = get_tree().get_root().size
	viewport_size = get_tree().get_root().get_visible_rect().size

func _ready() -> void:
	get_tree().on_request_permissions_result.connect(on_permisson)
	
func on_permisson(_per : String , state : bool) -> void:
	if state == false:
		get_tree().quit(1)
	
func menu() -> void:
	get_tree().change_scene_to_file("res://Scenes/Init/menu.tscn")
	
func play_click_sound() -> void:
	click.play()
