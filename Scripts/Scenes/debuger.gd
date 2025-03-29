extends CanvasLayer

var error_count : int = 0
var record_ms :int = 0

func _init() -> void:
	Engine.register_singleton("Debuger",self)

func error(text : String) -> void:
	error_count +=1
	print("[",Engine.get_physics_frames(),"]",text)
	
func record() -> void:
	if record_ms == 0:
		record_ms = Time.get_ticks_usec()
	else:
		print(Time.get_ticks_usec() - record_ms,"us")
		record_ms = 0

func output_time() -> void:
	print(Time.get_unix_time_from_system())

func _physics_process(delta: float) -> void:
	if Engine.get_physics_frames() % 60 == 0:
		%Label.text = ""
		%Label.text += str("FPS:",Performance.get_monitor(Performance.TIME_FPS),";")
		%Label.text += str("Nodes:",Performance.get_monitor(Performance.OBJECT_NODE_COUNT),";")
		%Label.text += str("Memory:",int(OS.get_static_memory_usage()/1024/1024),"MB;")
		%Label.text += str("Physic:",int(Performance.get_monitor(Performance.TIME_PHYSICS_PROCESS)/0.16*100),"%;")
		%Label.text += str("Error:",error_count,";")
		%Label.text += str("Draw Calls:",int(Performance.get_monitor(Performance.RENDER_TOTAL_DRAW_CALLS_IN_FRAME)),";")
		%Label.text += str("Draw Objects:",int(Performance.get_monitor(Performance.RENDER_TOTAL_PRIMITIVES_IN_FRAME)),";")
		
