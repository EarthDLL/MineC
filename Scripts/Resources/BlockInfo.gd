extends Resource
class_name BlockInf


enum CubeSide{
	UP = 0,
	DOWN = 1,
	WEST = 2,
	EAST = 3,
	NORTH = 4,
	SOUTH = 5
}

enum TextureType { 
	Normal,
	Custom
}
@export var id : int = 1
@export var typeid : String = ""

@export var is_cube := false
@export var custom_geometry : String = ""
@export var texture_type : TextureType = 0
@export var texture_pieces : Dictionary = {}
var texture_uv := {}

func translate_data() -> Array:
	var data := []
	return []
	
func deal_uv() -> void:
	if is_cube:
		var default : PackedVector2Array = ResManager.block_texture_uvs.miss
		if texture_pieces.has("*") && ResManager.block_texture_uvs.has(texture_pieces["*"]):
			default = ResManager.block_texture_uvs[texture_pieces["*"]]
		for piece : String in ResManager.cube_geo.textures:
			if texture_pieces.has(piece) && ResManager.block_texture_uvs.has(piece):
				texture_uv[piece] = ResManager.block_texture_uvs[piece]
			else:
				texture_uv[piece] = default
	texture_pieces = {}
