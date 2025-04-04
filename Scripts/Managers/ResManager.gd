extends Node

const block_info_dir = "res://Behaviours/Blocks/"
const texture_path = "res://Resources/Textures/"


var block_infos := {}
var block_ids := {}
var id_count := 2
var block_texture : ImageTexture = null
var block_material : StandardMaterial3D = load("res://Resources/Render/Blocks.tres")
var block_texture_uvs := {}

func _init() -> void:
	Engine.register_singleton("ResManager",self)

func get_block_info(id : int) -> BlockModelInfo:
	return block_infos.get(id,null)

func read_block_infos() -> void:
	var dir := DirAccess.open(block_info_dir)
	if is_instance_valid(dir):
		var names := dir.get_files()
		for file_name : String in names:
			if file_name.ends_with(".tres"):
				var res : BlockModelInfo = load(block_info_dir + file_name) as BlockModelInfo
				if is_instance_valid(res):
					block_ids[res.type_id] = id_count
					block_infos[id_count] = res
					id_count += 1
					res.deal_uv(block_texture_uvs)

func read_block_textures() -> void:
	var mix_tool := BlockMixImageTool.new()
	mix_tool.update()
	var description_file := FileAccess.open("res://Resources/Textures/Blocks.json",FileAccess.READ)
	if !is_instance_valid(description_file):
		return
	var dictionary : Dictionary = JSON.parse_string(description_file.get_as_text()) as Dictionary
	if dictionary.has("textures") && dictionary["textures"] is Dictionary:
		var textures : Dictionary = dictionary.textures
		for key in textures.keys():
			match(typeof(textures[key])):
				TYPE_STRING:
					var image = load(texture_path + textures[key])
					if image is Image:
						block_texture_uvs[key] = mix_tool.get_uv(mix_tool.add_image(image),0)
	block_texture = ImageTexture.create_from_image(mix_tool.mix_image)
	block_material.albedo_texture = block_texture
func _ready() -> void:
	read_block_textures()
	read_block_infos()
	
