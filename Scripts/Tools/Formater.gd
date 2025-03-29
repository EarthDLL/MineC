extends Object
class_name Formater

const geo_format := {
	"cube" :{
		"point" : Vector3(),
		"size" : Vector3(),
	}
}

## infos格式
## 行1 模型类型的列表,如["cube","cube"]
## 行2+ 每一面的顶点

#static func parse_geometry(geo : Geometry) -> Array:
#	var objects := geo.get_objects()
#	var infos : Array = []
#
#	for object in objects:
#		match object.get("type",""):
#			"cube":
#				Formater.dic_format(object,geo_format.cube)
#				var aabb := AABB(object.point,object.size)
#				infos.append(aabb)
#
#	return infos
	
## 格式化dic,format格式为{key : 默认值}，若key不存在或对应值的类型不一致则改为默认值
static func dic_format(dic : Dictionary , format : Dictionary):
	for key in format.keys():
		if dic.has(key):
			if typeof(dic[key]) != typeof(format[key]):
				dic[key] = format[key]
		else:
			dic[key] = format[key]
			
			
static func has_namespace(id : String) -> bool:
	if id.is_empty():
		return false
	if id.find(":",1) == -1:
		return false
	return true
	
static func path_import(path : String) -> String:
	if path.ends_with(".import"):
		return path.substr(0,path.length()-7)
	return path
	
##用于去除文件后缀名
static func get_file_name(path : String) -> String:
	path = path_import(path)
	var index := path.rfind(".")
	if index != -1:
		return path.substr(0,index)
	return path
	
	
	
	
