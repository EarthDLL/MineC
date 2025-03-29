extends RefCounted
class_name Chunk2

enum WorldType { 
	NORMAL  = 0 ,
	OLD = 1 ,
	FLAT = 2 ,
	CUSTOM = 3
}

const chunk_xz_size := 16
var chuck_id : Vector2i = Vector2i()
var chunk_size := Vector3i(16,256,16)

var region_count := 0
var blocks : PackedInt32Array = []
var render_data := {}
var is_loaded := false

func load_by_noise(noise : Noise) -> void:
	#print("Loading chunk...")
	Debuger.output_time()
	var start_point : Vector2i = chuck_id * chunk_xz_size
	for x in chunk_size.x:
		for y in chunk_size.z:
			var high : int = (clamp(noise.get_noise_2d(start_point.x + x , start_point.y + y)+0.8,-1,1)+1)*32
			for h in high:
				set_block(x,h,y,1)
	is_loaded = true
	#Debuger.output_time()

func _init(id : Vector2i = Vector2i(0,0),size : Vector3i = Vector3i(16,256,16)) -> void:
	chunk_size = size
	chuck_id = id
	blocks.resize(size.x*size.y*size.z)
	region_count = roundi(size.y/16)
	
func set_block(x : int , y : int ,z : int , id : int) -> void:
	blocks[y * chunk_size.x * chunk_size.z + chunk_size.x * x + z] = id
	
func get_block(x : int , y : int , z : int) -> int:
	return blocks[y * chunk_size.x * chunk_size.z + chunk_size.x * x + z]

var vertice := PackedVector3Array()
var uv := PackedVector2Array()

func render(region_index : int) -> bool:
	print("Render...")
	Debuger.output_time()
	if !is_loaded:
		return false
	vertice = PackedVector3Array()
	uv = PackedVector2Array()
	var array := Array()
	array.resize(Mesh.ARRAY_MAX)
	array[Mesh.ARRAY_VERTEX] = vertice
	array[Mesh.ARRAY_TEX_UV] = uv
	render_data[region_index] = array
	for x in chunk_size.x:
		for y in range(region_index * 16 , chunk_size.y):
			for z in chunk_size.z:
				var block := ResManager.get_block_info(get_block(x,y,z))
				if is_instance_valid(block):
					var side := get_block(x+1,y,z)
					if side == 0:
						draw_cube_side(Vector3i(x,y,z),block,BlockInfo.CubeSide.EAST)
					side = get_block(x-1,y,z)
					if side == 0:
						draw_cube_side(Vector3i(x,y,z),block,BlockInfo.CubeSide.WEST)
					side = get_block(x,y+1,z)
					if side == 0:
						draw_cube_side(Vector3i(x,y,z),block,BlockInfo.CubeSide.UP)
					side = get_block(x,y-1,z)
					if side == 0:
						draw_cube_side(Vector3i(x,y,z),block,BlockInfo.CubeSide.DOWN)
					side = get_block(x,y,z-1)
					if side == 0:
						draw_cube_side(Vector3i(x,y,z),block,BlockInfo.CubeSide.NORTH)
					side = get_block(x,y,z+1)
					if side == 0:
						draw_cube_side(Vector3i(x,y,z),block,BlockInfo.CubeSide.SOUTH)
	Debuger.output_time()
	return true

func draw_cube_side(pos : Vector3i , info : BlockInfo , side : BlockInfo.CubeSide) -> void:
	var position := Vector3(pos)
	match side:
		0:
			position.y += 1
			vertice.append(position)
			position.x += 1
			vertice.append(position)
			position.z += 1
			vertice.append(position)
			vertice.append(position)
			position.x -= 1
			vertice.append(position)
			position.z -= 1
			vertice.append(position)
			uv.append_array(info.texture_uv.up)
		1:
			position.x += 1
			position.z += 1
			vertice.append(position)
			position.z -= 1
			vertice.append(position)
			position.x -= 1
			vertice.append(position)
			vertice.append(position)
			position.z += 1
			vertice.append(position)
			position.x += 1
			vertice.append(position)
			uv.append_array(info.texture_uv.down)
		2:
			position.y += 1
			vertice.append(position)
			position.z += 1
			vertice.append(position)
			position.y -= 1
			vertice.append(position)
			vertice.append(position)
			position.z -= 1
			vertice.append(position)
			position.y += 1
			vertice.append(position)
			uv.append_array(info.texture_uv.west)
		3:
			position += Vector3(1,1,1)
			vertice.append(position)
			position.z -= 1
			vertice.append(position)
			position.y -= 1
			vertice.append(position)
			vertice.append(position)
			position.z += 1
			vertice.append(position)
			position.y += 1
			vertice.append(position)
			uv.append_array(info.texture_uv.east)
		4:
			position += Vector3(1,1,0)
			vertice.append(position)
			position.x -= 1
			vertice.append(position)
			position.y -= 1
			vertice.append(position)
			vertice.append(position)
			position.x += 1
			vertice.append(position)
			position.y += 1
			vertice.append(position)
			uv.append_array(info.texture_uv.north)
		5:
			position += Vector3(0,1,1)
			vertice.append(position)
			position.x += 1
			vertice.append(position)
			position.y -= 1
			vertice.append(position)
			vertice.append(position)
			position.x -= 1
			vertice.append(position)
			position.y += 1
			vertice.append(position)
			uv.append_array(info.texture_uv.south)


#func _init(noise : Noise = null,index : Vector2i = Vector2i(0,0)) -> void:
	#var start_point := Vector3i(index.x * chunk_size.x , 0 , index.y * chunk_size.z)
	#print(start_point)
	#arrays.resize(Mesh.ArrayType.ARRAY_MAX)
	#for x in chunk_size.x:
		#for z in chunk_size.z:
			#var high : int = roundf((clamp(noise.get_noise_2d(start_point.x + x , start_point.z + z)+0.8,-1,1)+1)*32)
			#for y in high:
				#blocks[x][y][z] = 2
#
#
	##0-下 1-上 2-X小 3-X大
#func draw_full_block_side(location:Vector3i , id :int ,side : int):
		#var texture_pos := PackedVector2Array()
		#texture_pos.resize(4)
		##Dictionary piece = (Dictionary)((GodotObject)block_infos[id]).Get("texture_pieces");
		#location = start_point + location
		#match side:
			#0:
				#location.x += 1;
				#vertices.append(location)
				#location.x -= 1;
				#vertices.append(location)
				#location.z += 1;
				#vertices.append(location)
				#vertices.append(location)
				#location.x += 1;
				#vertices.append(location)
				#location.z -= 1;
				#vertices.append(location)
				#
				#uvs.append_array(block_textures[id]["up"])
			#1:
				#location.y += 1;
				#vertices.append(location)
				#location.x += 1;
				#vertices.append(location)
				#location.z += 1;
				#vertices.append(location)
				#vertices.append(location)
				#location.x -= 1;
				#vertices.append(location)
				#location.z -= 1;
				#vertices.append(location)
				#
				#uvs.append_array(block_textures[id]["down"])
			#3:
				#location += Vector3i(1,1,1);
				#vertices.append(location)
				#location.z -= 1;
				#vertices.append(location)
				#location.y -= 1;
				#vertices.append(location)
				#vertices.append(location)
				#location.z += 1;
				#vertices.append(location)
				#location.y += 1;
				#vertices.append(location)
				#uvs.append_array(block_textures[id]["east"])
			#2:
				#location.y += 1;
				#vertices.append(location)
				#location.z += 1;
				#vertices.append(location)
				#location.y -= 1;
				#vertices.append(location)
				#vertices.append(location)
				#location.z -= 1;
				#vertices.append(location)
				#location.y += 1;
				#vertices.append(location)
				#uvs.append_array(block_textures[id]["west"])
			#4:
				#location.x += 1;
				#location.y += 1;
				#vertices.append(location)
				#location.x -= 1;
				#vertices.append(location)
				#location.y -= 1;
				#vertices.append(location)
				#vertices.append(location)
				#location.x += 1;
				#vertices.append(location)
				#location.y += 1;
				#vertices.append(location)
				#uvs.append_array(block_textures[id]["north"])
			#5:
				#location.y += 1;
				#location.z += 1;
				#vertices.append(location)
				#location.x += 1;
				#vertices.append(location)
				#location.y -= 1;
				#vertices.append(location)
				#vertices.append(location)
				#location.x -= 1;
				#vertices.append(location)
				#location.y += 1;
				#vertices.append(location)
				#uvs.append_array(block_textures[id]["south"])
#
#
#func render(pieces:Dictionary) -> Mesh:
		#block_textures = pieces
		#var mesh = ArrayMesh.new()
		#for x in chunk_size.x:
			#for z in chunk_size.z:
				#for y in chunk_size.y:
					#var block_id : int = blocks[x][y][z]
					#if block_id != 0:
						#if y > 0:
							#if blocks[x][y-1][z] == 0:
								#draw_full_block_side(Vector3i(x,y,z) , block_id , 0)
						#else:
							#draw_full_block_side(Vector3i(x,y,z) , block_id , 0)
						#if y < chunk_size.y -1:
							#if blocks[x][y+1][z] == 0:
								#draw_full_block_side(Vector3i(x,y,z) , block_id , 1)
						#else:
							#draw_full_block_side(Vector3i(x,y,z) , block_id , 1)
#
						#if x > 0:
							#if blocks[x-1][y][z] == 0:
								#draw_full_block_side(Vector3i(x,y,z) ,block_id , 2)
						#else:
							#draw_full_block_side(Vector3i(x,y,z) , block_id , 2)
						#if x < chunk_size.x -1:
							#if blocks[x+1][y][z] == 0:
								#draw_full_block_side(Vector3i(x,y,z) ,block_id, 3)
						#else:
							#draw_full_block_side(Vector3i(x,y,z) ,block_id, 3)
							#
						#if z > 0:
							#if blocks[x][y][z-1] == 0:
								#draw_full_block_side(Vector3i(x,y,z) ,block_id ,4)
						#else:
							#draw_full_block_side(Vector3i(x,y,z) ,block_id, 4)
						#if z < chunk_size.z -1:
							#if blocks[x][y][z+1] == 0:
								#draw_full_block_side(Vector3i(x,y,z) ,block_id ,5)
						#else:
							#draw_full_block_side(Vector3i(x,y,z) ,block_id ,5)
#
		#arrays[Mesh.ArrayType.ARRAY_VERTEX] = PackedVector3Array(vertices)
		#arrays[Mesh.ArrayType.ARRAY_TEX_UV] = PackedVector2Array(uvs)
		#mesh.add_surface_from_arrays(Mesh.PrimitiveType.PRIMITIVE_TRIANGLES,arrays)
		#vertices.clear()
		#uvs.clear()
#
		#return mesh
