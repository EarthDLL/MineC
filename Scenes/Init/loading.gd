extends Control

@onready var progress: ProgressBar = $Progress
@export var next : PackedScene = null
var current : float = 0

var able : bool = false
var jumpable : bool = false

func _ready() -> void:
	ResManager.Complete_load.connect(func():
		jumpable = true
		)
	get_tree().create_timer(0.5).timeout.connect(func():
		able = true
		)
	OS.request_permissions()

func _physics_process(_delta: float) -> void:
	if able:
		current = ResManager.get_load_progress()
		progress.value += min(0.03,current-progress.value)
	if current == progress.value && jumpable:
		jump()



func jump()->void:
	if next != null:
		Manager.menu()
