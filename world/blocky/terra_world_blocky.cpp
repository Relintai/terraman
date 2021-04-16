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

#include "terra_world_blocky.h"

#include "terra_chunk_blocky.h"

#include "../../meshers/blocky/terra_mesher_blocky.h"
#include "../../meshers/blocky/terra_mesher_liquid_blocky.h"
#include "../jobs/terra_light_job.h"
#include "../jobs/terra_prop_job.h"
#include "../jobs/terra_terrarin_job.h"

Ref<TerraChunk> TerraWorldBlocky::_create_chunk(int x, int z, Ref<TerraChunk> chunk) {

	if (!chunk.is_valid()) {
		chunk = Ref<TerraChunk>(memnew(TerraChunkBlocky));
	}

	if (chunk->job_get_count() == 0) {
		Ref<TerraTerrarinJob> tj;
		tj.instance();

		Ref<TerraLightJob> lj;
		lj.instance();

		Ref<TerraPropJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<TerraMesher>(memnew(TerraMesherBlocky)));

		tj->add_mesher(Ref<TerraMesher>(memnew(TerraMesherBlocky())));
		tj->add_liquid_mesher(Ref<TerraMesher>(memnew(TerraMesherLiquidBlocky())));

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	return TerraWorld::_create_chunk(x, z, chunk);
}

TerraWorldBlocky::TerraWorldBlocky() {
	set_data_margin_start(1);
	set_data_margin_end(1);
}

TerraWorldBlocky ::~TerraWorldBlocky() {
}

void TerraWorldBlocky::_bind_methods() {
}