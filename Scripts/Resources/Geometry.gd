extends Resource
class_name Geometry

@export var id := "geo.unknown"
@export var textures : Array[String] = []

func get_id() -> String:
	return id
