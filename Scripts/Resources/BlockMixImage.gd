extends RefCounted
class_name BlockMixImageTool

const uv_sort = [Vector2(0,0),Vector2(1,0),Vector2(1,1),Vector2(0,1)]

enum UvDirection{
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3
}

var base_size : Vector2i = Vector2i(16,16)
var image_size : Vector2i = Vector2i(512,512)
var count : Vector2i = Vector2i()
var current_images := 0
var max_images := 0
var is_valid := false
var mix_image : Image = null

func update() -> void:
	if image_size.x % base_size.x == 0 && image_size.y % base_size.y == 0:
		is_valid = true
		max_images = image_size.x * image_size.y / base_size.x / base_size.y
		mix_image = Image.create_empty(image_size.x,image_size.y,false,Image.FORMAT_RGB8)
		count = image_size/base_size
		
func add_image(image : Image) -> Vector2i:
	if !is_valid:
		return Vector2i(-1,0)
	if current_images >= max_images || image.get_size() != base_size:
		return Vector2i(-1,0)
	var pos := Vector2i(current_images%count.x*base_size.x,current_images/count.x*base_size.y)
	if image.get_format() != Image.FORMAT_RGB8:
		image.convert(Image.FORMAT_RGB8)
	mix_image.blit_rect(image,Rect2(Vector2(0,0),Vector2(base_size)),pos)
	current_images += 1
	return pos
	
func get_uv(pos : Vector2 , direction : UvDirection) -> PackedVector2Array:
	var array : PackedVector2Array = []
	for index : int in [0,1,2,2,3,0]:
		array.append((pos + uv_sort[(index + direction)%4] * Vector2(base_size)) / Vector2(image_size))
	return array
	
	
