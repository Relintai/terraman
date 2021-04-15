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

#ifndef TERRA_MESHER_MARCHING_CUBES_H
#define TERRA_MESHER_MARCHING_CUBES_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/reference.h"
#else
#include "core/reference.h"
#endif

#include "../default/voxel_mesher_default.h"

#include "marching_cubes_cell_data.h"

#include "marching_cubes_tables.h"

using namespace MarchingCubes;

class TerraMesherMarchingCubes : public TerraMesherDefault {
	GDCLASS(TerraMesherMarchingCubes, TerraMesherDefault)

public:
	static const String BINDING_STRING_TERRA_ENTRY_INDICES;
	static const String BINDING_STRING_TERRA_ENTRY_MASK;

	enum TerraEntryIndices {
		TERRA_ENTRY_INDEX_000 = 0,
		TERRA_ENTRY_INDEX_100 = 1,
		TERRA_ENTRY_INDEX_001 = 2,
		TERRA_ENTRY_INDEX_101 = 3,

		TERRA_ENTRY_INDEX_010 = 4,
		TERRA_ENTRY_INDEX_110 = 5,
		TERRA_ENTRY_INDEX_011 = 6,
		TERRA_ENTRY_INDEX_111 = 7,

		TERRA_ENTRIES_SIZE = 8,
	};

	enum TerraEntryMask {
		TERRA_ENTRY_MASK_000 = 1 << 0,
		TERRA_ENTRY_MASK_100 = 1 << 1,
		TERRA_ENTRY_MASK_001 = 1 << 2,
		TERRA_ENTRY_MASK_101 = 1 << 3,

		TERRA_ENTRY_MASK_010 = 1 << 4,
		TERRA_ENTRY_MASK_110 = 1 << 5,
		TERRA_ENTRY_MASK_011 = 1 << 6,
		TERRA_ENTRY_MASK_111 = 1 << 7,
	};

	//arr should have a size of 8
	void get_voxel_type_array(int *arr, Ref<TerraChunk> chunk, const int x, const int y, const int z, const int size = 1);
	int get_case_code_from_arr(const int *data);
	int get_case_code(Ref<TerraChunk> chunk, const int x, const int y, const int z, const int size = 1);
	int get_voxel_type(Ref<TerraChunk> chunk, const int x, const int y, const int z, const int size = 1);
	void _add_chunk(Ref<TerraChunk> p_chunk);

	Vector3 corner_id_to_vertex(int corner_id) const;

	int get_regular_cell_class(int index) const;

	Ref<MarchingCubesCellData> get_regular_cell_data(int index) const;
	int get_regular_vertex_data(int index10, int index2) const;
	// void set_regular_vertex_data(int index1, int index2, int value);
	int get_regular_vertex_data_first_vertex(int index1, int index2) const;
	int get_regular_vertex_data_second_vertex(int index1, int index2) const;
	Vector3 get_regular_vertex_first_position(int index1, int index2) const;
	Vector3 get_regular_vertex_second_position(int index1, int index2) const;
	Vector3 get_regular_vertex_direction(int index1, int index2) const;

	TerraMesherMarchingCubes();
	~TerraMesherMarchingCubes();

protected:
	static void _bind_methods();

	Ref<MarchingCubesCellData> _regular_cell_datas[16];
};

VARIANT_ENUM_CAST(TerraMesherMarchingCubes::TerraEntryIndices);
VARIANT_ENUM_CAST(TerraMesherMarchingCubes::TerraEntryMask);

#endif // TERRA_MESHER_SMOOTH_H
