extends Control
class_name JoyStick

@onready var stick: TextureRect = $Stick
@onready var joy: TextureRect = $Joy
@onready var center := joy.get_rect().get_center()
var move : Vector2

func _ready() -> void:
	joy.resized.connect(func ():
		center = joy.get_rect().get_center()
		)

func _gui_input(event: InputEvent) -> void:
	if event is InputEventScreenDrag:
		if event.index == stick.id:
			event = joy.make_input_local(event)
			move = (event.get_position() - center - stick.offest).clampf(-joy.size.x/2,joy.size.x/2)
			stick.position = move + center - stick.size/2 
			
func reset_stick() -> void:
	stick.position = center - stick.size/2
	move = Vector2.ZERO
	
func get_movement() -> Vector2:
	return move.normalized()
