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

#include "library/terra_surface.h"
#include "library/terra_surface_simple.h"

#include "library/terra_material_cache.h"
#include "library/terraman_library.h"
#include "library/terraman_library_simple.h"

#ifdef TEXTURE_PACKER_PRESENT
#include "library/terra_surface_merger.h"
#include "library/terraman_library_merger.h"
#include "library/terraman_library_merger_pcm.h"
#include "library/terra_material_cache_pcm.h"
#endif

#include "data/terra_light.h"
#include "meshers/terra_mesher.h"

#include "world/block_terra_structure.h"
#include "world/terra_chunk.h"
#include "world/terra_environment_data.h"
#include "world/terra_structure.h"
#include "world/terra_world.h"

#include "world/default/terra_chunk_default.h"
#include "world/default/terra_world_default.h"

#include "level_generator/terraman_level_generator.h"
#include "level_generator/terraman_level_generator_flat.h"

#include "areas/terra_world_area.h"

#include "world/terra_world_editor.h"

#include "meshers/blocky/terra_mesher_blocky.h"
#include "meshers/blocky/terra_mesher_liquid_blocky.h"

#include "world/blocky/terra_chunk_blocky.h"
#include "world/blocky/terra_world_blocky.h"

#include "nodes/terraman_light.h"

#include "world/jobs/terra_job.h"
#include "world/jobs/terra_light_job.h"
#include "world/jobs/terra_mesher_job_step.h"
#include "world/jobs/terra_prop_job.h"
#include "world/jobs/terra_terrarin_job.h"

void register_terraman_types() {
	ClassDB::register_class<TerraMesher>();
	ClassDB::register_class<TerraMesherDefault>();

	ClassDB::register_class<TerraSurface>();
	ClassDB::register_class<TerraSurfaceSimple>();

	ClassDB::register_class<TerramanLibrary>();
	ClassDB::register_class<TerramanLibrarySimple>();

	ClassDB::register_class<TerraMaterialCache>();

#ifdef TEXTURE_PACKER_PRESENT
	ClassDB::register_class<TerraSurfaceMerger>();
	ClassDB::register_class<TerramanLibraryMerger>();
	ClassDB::register_class<TerramanLibraryMergerPCM>();
	ClassDB::register_class<TerraMaterialCachePCM>();
#endif

	ClassDB::register_class<TerraLight>();
	ClassDB::register_class<TerramanLight>();

	ClassDB::register_class<TerraWorld>();
	ClassDB::register_class<TerraChunk>();
	ClassDB::register_class<TerraStructure>();
	ClassDB::register_class<BlockTerraStructure>();
	ClassDB::register_class<TerraEnvironmentData>();

	ClassDB::register_class<TerraChunkDefault>();
	ClassDB::register_class<TerraWorldDefault>();

	ClassDB::register_class<TerraMesherBlocky>();
	ClassDB::register_class<TerraWorldBlocky>();
	ClassDB::register_class<TerraChunkBlocky>();
	ClassDB::register_class<TerraMesherLiquidBlocky>();

	ClassDB::register_class<TerramanLevelGenerator>();
	ClassDB::register_class<TerramanLevelGeneratorFlat>();

	ClassDB::register_class<TerraWorldArea>();

	ClassDB::register_class<TerraJob>();
	ClassDB::register_class<TerraTerrarinJob>();
	ClassDB::register_class<TerraMesherJobStep>();
	ClassDB::register_class<TerraLightJob>();
	ClassDB::register_class<TerraPropJob>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<TerraWorldEditorPlugin>();
#endif
}

void unregister_terraman_types() {
}
