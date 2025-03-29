extends CharacterBody3D
class_name Player

@onready var label_3d: Label3D = $Label3D
@export var joy : JoyStick = null
@export var jump : JoyBtn = null
@export var sneak : JoyBtn = null
@export var view_range : int = 2

const SPEED = 18.0
const JUMP_VELOCITY = 8

# Get the gravity from the project settings to be synced with RigidBody nodes.
var gravity: float = 9

var input_speed : float = 0.4
var input_id :int = -1

func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventScreenTouch:
		if input_id == -1 && event.is_pressed():
			input_id = event.index
		if input_id == event.index && !event.is_pressed():
			input_id = -1
	elif event is InputEventScreenDrag:
		if event.index == input_id:
			var offest:Vector2 = event.relative
			rotation_degrees.y += -offest.x * input_speed
			rotation_degrees.x += -offest.y * input_speed


func _physics_process(delta: float) -> void:
	label_3d.text = str(Vector3i(position))
	

	# Handle Jump.
	#if Input.is_action_pressed("ui_accept"):
		#velocity.y = JUMP_VELOCITY * 3
	#elif Input.is_action_pressed("ui_shift"):
		#velocity.y = -JUMP_VELOCITY * 3
	#else :
		#velocity.y = 0

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var input_dir := Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	#var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	#if direction:
		#velocity.x = direction.x * SPEED
		#velocity.z = direction.z * SPEED
	#else:
		#velocity.x = move_toward(velocity.x, 0, SPEED)
		#velocity.z = move_toward(velocity.z, 0, SPEED)
	if jump != null:
		if jump.is_pressed():
			velocity.y = SPEED
		else:
			velocity.y = 0
			if sneak != null:
				if sneak.is_pressed():
					velocity.y = -SPEED
				else:
					velocity.y = 0
	if joy != null:
		var side := Vector2.from_angle(rotation.y) * joy.get_movement().y * SPEED
		side += Vector2.from_angle(rotation.y + PI/2) * joy.get_movement().x * SPEED
		velocity.z = side.x
		velocity.x = side.y

	move_and_slide()
