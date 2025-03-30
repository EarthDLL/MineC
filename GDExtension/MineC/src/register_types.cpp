/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "Chunk.h"
#include "BlockInfo.h"
#include "RenderTool.h"
#include "ChunkManager.h"
#include "RenderManager.h"

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(Chunk);
	GDREGISTER_CLASS(BlockInfo);
	GDREGISTER_CLASS(RenderTool);
	GDREGISTER_CLASS(ChunkManager);
	GDREGISTER_CLASS(RenderManager);
	/*GDREGISTER_CLASS(ExampleRef);
	GDREGISTER_CLASS(ExampleMin);
	GDREGISTER_CLASS(Example);
	GDREGISTER_VIRTUAL_CLASS(ExampleVirtual);
	GDREGISTER_ABSTRACT_CLASS(ExampleAbstractBase);
	GDREGISTER_CLASS(ExampleConcrete);
	GDREGISTER_CLASS(ExampleBase);
	GDREGISTER_CLASS(ExampleChild);
	GDREGISTER_RUNTIME_CLASS(ExampleRuntime);*/
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT gdextension_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_example_module);
	init_obj.register_terminator(uninitialize_example_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
