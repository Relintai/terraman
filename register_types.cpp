/*
Copyright (c) 2019-2021 Péter Magyar

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


#include "library/voxel_surface.h"
#include "library/voxel_surface_simple.h"

#include "library/voxelman_library.h"
#include "library/voxelman_library_simple.h"

#ifdef TEXTURE_PACKER_PRESENT
#include "library/voxel_surface_merger.h"
#include "library/voxelman_library_merger.h"
#endif

#include "data/voxel_light.h"
#include "meshers/voxel_mesher.h"

#include "meshers/marching_cubes/marching_cubes_cell_data.h"
#include "meshers/marching_cubes/voxel_mesher_marching_cubes.h"

#include "world/block_voxel_structure.h"
#include "world/environment_data.h"
#include "world/voxel_chunk.h"
#include "world/voxel_structure.h"
#include "world/voxel_world.h"

#include "world/default/voxel_chunk_default.h"
#include "world/default/voxel_world_default.h"

#include "meshers/cubic/voxel_cube_points.h"
#include "meshers/cubic/voxel_mesher_cubic.h"

#include "level_generator/voxelman_level_generator.h"
#include "level_generator/voxelman_level_generator_flat.h"

#include "areas/world_area.h"

#include "world/voxel_world_editor.h"

#include "meshers/blocky/voxel_mesher_blocky.h"
#include "meshers/blocky/voxel_mesher_liquid_blocky.h"

#include "world/blocky/voxel_chunk_blocky.h"
#include "world/blocky/voxel_world_blocky.h"

#include "world/cubic/voxel_chunk_cubic.h"
#include "world/cubic/voxel_world_cubic.h"

#include "world/marching_cubes/voxel_chunk_marching_cubes.h"
#include "world/marching_cubes/voxel_world_marching_cubes.h"

#include "nodes/voxelman_light.h"

#include "world/jobs/voxel_job.h"
#include "world/jobs/voxel_light_job.h"
#include "world/jobs/voxel_prop_job.h"
#include "world/jobs/voxel_terrarin_job.h"


void register_terraman_types() {
	ClassDB::register_class<TerraMesher>();
	ClassDB::register_class<TerraMesherDefault>();

	ClassDB::register_class<TerraMesherMarchingCubes>();
	ClassDB::register_class<MarchingCubesCellData>();

	ClassDB::register_class<TerraSurface>();
	ClassDB::register_class<TerraSurfaceSimple>();

	ClassDB::register_class<TerramanLibrary>();
	ClassDB::register_class<TerramanLibrarySimple>();

#ifdef TEXTURE_PACKER_PRESENT
	ClassDB::register_class<TerraSurfaceMerger>();
	ClassDB::register_class<TerramanLibraryMerger>();
#endif

	ClassDB::register_class<TerraLight>();
	ClassDB::register_class<TerramanLight>();

	ClassDB::register_class<TerraWorld>();
	ClassDB::register_class<TerraChunk>();
	ClassDB::register_class<TerraStructure>();
	ClassDB::register_class<BlockTerraStructure>();
	ClassDB::register_class<EnvironmentData>();

	ClassDB::register_class<TerraChunkDefault>();
	ClassDB::register_class<TerraWorldDefault>();

	ClassDB::register_class<TerraMesherCubic>();
	ClassDB::register_class<TerraCubePoints>();

	ClassDB::register_class<TerraMesherBlocky>();
	ClassDB::register_class<TerraWorldBlocky>();
	ClassDB::register_class<TerraChunkBlocky>();
	ClassDB::register_class<TerraMesherLiquidBlocky>();

	ClassDB::register_class<TerraWorldMarchingCubes>();
	ClassDB::register_class<TerraChunkMarchingCubes>();

	ClassDB::register_class<TerraWorldCubic>();
	ClassDB::register_class<TerraChunkCubic>();

	ClassDB::register_class<TerramanLevelGenerator>();
	ClassDB::register_class<TerramanLevelGeneratorFlat>();

	ClassDB::register_class<TerraWorldArea>();

	ClassDB::register_class<TerraJob>();
	ClassDB::register_class<TerraTerrarinJob>();
	ClassDB::register_class<TerraLightJob>();
	ClassDB::register_class<TerraPropJob>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<TerraWorldEditorPlugin>();
#endif
}

void unregister_terraman_types() {
}
