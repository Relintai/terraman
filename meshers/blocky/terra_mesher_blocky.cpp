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

#include "terra_mesher_blocky.h"

#include "../../world/default/terra_chunk_default.h"

bool TerraMesherBlocky::get_always_add_colors() const {
	return _always_add_colors;
}
void TerraMesherBlocky::set_always_add_colors(const bool value) {
	_always_add_colors = value;
}

int TerraMesherBlocky::get_lod_index() const {
	return _lod_index;
}
void TerraMesherBlocky::set_lod_index(const int value) {
	_lod_index = value;
}

void TerraMesherBlocky::_add_chunk(Ref<TerraChunk> p_chunk) {
	Ref<TerraChunkDefault> chunk = p_chunk;

	ERR_FAIL_COND(!chunk.is_valid());

	//if ((get_build_flags() & TerraChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerraChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int z_size = chunk->get_size_z();
	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerraChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerraChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerraChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerraChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerraChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	int lod_skip = 1;

	if (_lod_index > 0) {
		lod_skip = _lod_index * 2;
	}

	for (int z = chunk->get_margin_start(); z < z_size + chunk->get_margin_end(); z += lod_skip) {
		for (int x = chunk->get_margin_start(); x < x_size + chunk->get_margin_end(); x += lod_skip) {

			int indexes[4] = {
				chunk->get_data_index(x + lod_skip, z),
				chunk->get_data_index(x, z),
				chunk->get_data_index(x, z + lod_skip),
				chunk->get_data_index(x + lod_skip, z + lod_skip)
			};

			uint8_t type = channel_type[indexes[0]];

			if (type == 0)
				continue;

			Ref<TerraSurface> surface = _library->voxel_surface_get(type - 1);

			if (!surface.is_valid())
				continue;

			uint8_t isolevels[] = {
				channel_isolevel[indexes[0]],
				channel_isolevel[indexes[1]],
				channel_isolevel[indexes[2]],
				channel_isolevel[indexes[3]],
			};

			if (use_lighting) {
				for (int i = 0; i < 4; ++i) {
					int indx = indexes[i];

					light[i] = Color(channel_color_r[indx] / 255.0,
							channel_color_g[indx] / 255.0,
							channel_color_b[indx] / 255.0);

					float ao = 0;

					if (use_ao)
						ao = channel_ao[indx] / 255.0;

					if (use_rao) {
						float rao = channel_rao[indx] / 255.0;
						ao += rao;
					}

					light[i] += base_light;

					if (ao > 0)
						light[i] -= Color(ao, ao, ao) * _ao_strength;

					light[i].r = CLAMP(light[i].r, 0, 1.0);
					light[i].g = CLAMP(light[i].g, 0, 1.0);
					light[i].b = CLAMP(light[i].b, 0, 1.0);
				}
			}

			int vc = get_vertex_count();
			add_indices(vc + 2);
			add_indices(vc + 1);
			add_indices(vc + 0);
			add_indices(vc + 3);
			add_indices(vc + 2);
			add_indices(vc + 0);

			Vector2 uvs[] = {
				surface->transform_uv_scaled(TerraSurface::TERRA_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerraSurface::TERRA_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerraSurface::TERRA_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerraSurface::TERRA_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
			};

			Vector3 verts[] = {
				Vector3(x + lod_skip, isolevels[0] / 255.0 * world_height, z) * voxel_scale,
				Vector3(x, isolevels[1] / 255.0 * world_height, z) * voxel_scale,
				Vector3(x, isolevels[2] / 255.0 * world_height, z + lod_skip) * voxel_scale,
				Vector3(x + lod_skip, isolevels[3] / 255.0 * world_height, z + lod_skip) * voxel_scale
			};

			Vector3 normals[] = {
				(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
				(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
				(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
				(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
			};

			for (int i = 0; i < 4; ++i) {
				add_normal(normals[i]);

				if (use_lighting || _always_add_colors)
					add_color(light[i]);

				add_uv(uvs[i]);
				add_vertex(verts[i]);
			}
		}
	}
}

TerraMesherBlocky::TerraMesherBlocky() {
	_always_add_colors = false;
	_lod_index = 0;
}

TerraMesherBlocky::~TerraMesherBlocky() {
}

void TerraMesherBlocky::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_chunk", "buffer"), &TerraMesherBlocky::_add_chunk);

	ClassDB::bind_method(D_METHOD("get_always_add_colors"), &TerraMesherBlocky::get_always_add_colors);
	ClassDB::bind_method(D_METHOD("set_always_add_colors", "value"), &TerraMesherBlocky::set_always_add_colors);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "always_add_colors"), "set_always_add_colors", "get_always_add_colors");

	ClassDB::bind_method(D_METHOD("get_lod_index"), &TerraMesherBlocky::get_lod_index);
	ClassDB::bind_method(D_METHOD("set_lod_index", "value"), &TerraMesherBlocky::set_lod_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_index"), "set_lod_index", "get_lod_index");
}
