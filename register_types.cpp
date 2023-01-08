/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "register_types.h"

#include "library/terrain_surface.h"
#include "library/terrain_surface_simple.h"

#include "library/terrain_library.h"
#include "library/terrain_library_simple.h"
#include "library/terrain_material_cache.h"

#ifdef TEXTURE_PACKER_PRESENT
#include "library/terrain_library_merger.h"
#include "library/terrain_library_merger_pcm.h"
#include "library/terrain_material_cache_pcm.h"
#include "library/terrain_surface_merger.h"
#endif

#include "data/terrain_light.h"
#include "meshers/terrain_mesher.h"

#include "world/block_terrain_structure.h"
#include "world/terrain_chunk.h"
#include "world/terrain_environment_data.h"
#include "world/terrain_structure.h"
#include "world/terrain_world.h"

#include "world/default/terrain_chunk_default.h"
#include "world/default/terrain_world_default.h"

#include "level_generator/terrain_level_generator.h"
#include "level_generator/terrain_level_generator_flat.h"

#include "areas/terrain_world_area.h"

#include "world/terrain_world_editor.h"

#include "meshers/blocky/terrain_mesher_blocky.h"

#include "world/blocky/terrain_chunk_blocky.h"
#include "world/blocky/terrain_world_blocky.h"

#include "nodes/terrain_light_node.h"

#include "world/jobs/terrain_job.h"
#include "world/jobs/terrain_light_job.h"
#include "world/jobs/terrain_mesher_job_step.h"
#include "world/jobs/terrain_prop_job.h"
#include "world/jobs/terrain_terrain_job.h"

void initialize_terraman_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(TerrainMesher);
		GDREGISTER_CLASS(TerrainMesherDefault);

		GDREGISTER_CLASS(TerrainSurface);
		GDREGISTER_CLASS(TerrainSurfaceSimple);

		GDREGISTER_CLASS(TerrainLibrary);
		GDREGISTER_CLASS(TerrainLibrarySimple);

		GDREGISTER_CLASS(TerrainMaterialCache);

#ifdef TEXTURE_PACKER_PRESENT
		GDREGISTER_CLASS(TerrainSurfaceMerger);
		GDREGISTER_CLASS(TerrainLibraryMerger);
		GDREGISTER_CLASS(TerrainLibraryMergerPCM);
		GDREGISTER_CLASS(TerrainMaterialCachePCM);
#endif

		GDREGISTER_CLASS(TerrainLight);
		GDREGISTER_CLASS(TerrainLightNode);

		GDREGISTER_CLASS(TerrainWorld);
		GDREGISTER_CLASS(TerrainChunk);
		GDREGISTER_CLASS(TerrainStructure);
		GDREGISTER_CLASS(BlockTerrainStructure);
		GDREGISTER_CLASS(TerrainEnvironmentData);

		GDREGISTER_CLASS(TerrainChunkDefault);
		GDREGISTER_CLASS(TerrainWorldDefault);

		GDREGISTER_CLASS(TerrainMesherBlocky);
		GDREGISTER_CLASS(TerrainWorldBlocky);
		GDREGISTER_CLASS(TerrainChunkBlocky);

		GDREGISTER_CLASS(TerrainLevelGenerator);
		GDREGISTER_CLASS(TerrainLevelGeneratorFlat);

		GDREGISTER_CLASS(TerrainWorldArea);

		GDREGISTER_CLASS(TerrainJob);
		GDREGISTER_CLASS(TerrainTerrainJob);
		GDREGISTER_CLASS(TerrainMesherJobStep);
		GDREGISTER_CLASS(TerrainLightJob);
		GDREGISTER_CLASS(TerrainPropJob);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<TerrainWorldEditorPlugin>();
	}
#endif
}

void uninitialize_terraman_module(ModuleInitializationLevel p_level) {
}
