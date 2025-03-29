extends ControlJoy
class_name TouchJoy

@onready var left: JoyBtn = $Direction/Left
@onready var snap: JoyBtn = $Direction/Snap
@onready var down: JoyBtn = $Direction/Down
@onready var up: JoyBtn = $Direction/Up
@onready var right: JoyBtn = $Direction/Right

func get_movement() -> Vector3:
	var value := Vector3()
	if left.get_state():
		value.x -= 1
	if right.get_state():
		value.x += 1
	if down.get_state():
		value.z += 1
	if up.get_state():
		value.z -= 1
	return value
