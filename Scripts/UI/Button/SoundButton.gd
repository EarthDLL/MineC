extends Button
class_name SoundButton

func _init() -> void:
	pressed.connect(func():
		Manager.play_click_sound())
