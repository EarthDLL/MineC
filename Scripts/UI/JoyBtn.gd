extends Button
class_name JoyBtn

enum DealMode{
	TOUCHMODE = 0,
	TOGGLEMODE = 1,
	DOUBLEMODE = 2
}

var index : int = -1
@export var mode : DealMode = 0

func _init() -> void:
	button_mask = 0
	toggle_mode = true
	
func _gui_input(event: InputEvent) -> void:
	if event is InputEventScreenTouch:
		if index == -1 && event.is_pressed():
			btn_pressed(true,event.get_index(),event.is_double_tap())
		elif !event.is_pressed():
			btn_pressed(false,event.get_index(),event.is_double_tap())

func btn_pressed(action : bool , id : int , double : bool):
	match mode:
		0:
			if action:
				index = id
				set_pressed(true)
			elif !action && index == id:
				set_pressed(false)
				index = -1
		1:
			if action:
				set_pressed(!is_pressed())
		2:
			if double:
				set_pressed(!is_pressed())
