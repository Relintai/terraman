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

#include "terra_terrarin_job.h"

#include "../../library/terra_surface.h"
#include "../../library/terraman_library.h"

#include "../../meshers/default/terra_mesher_default.h"
#include "../../meshers/terra_mesher.h"

#include "../default/terra_chunk_default.h"

#ifdef MESH_UTILS_PRESENT
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

Ref<TerraMesher> TerraTerrarinJob::get_mesher() const {
	return _mesher;
}
void TerraTerrarinJob::set_mesher(const Ref<TerraMesher> &mesher) {
	_mesher = mesher;
}

Ref<TerraMesher> TerraTerrarinJob::get_liquid_mesher() const {
	return _liquid_mesher;
}
void TerraTerrarinJob::set_liquid_mesher(const Ref<TerraMesher> &mesher) {
	_liquid_mesher = mesher;
}

void TerraTerrarinJob::phase_setup() {
	if (_mesher.is_valid()) {
		_mesher->set_library(_chunk->get_library());
		_mesher->reset();
	}

	if (_liquid_mesher.is_valid()) {
		_liquid_mesher->set_library(_chunk->get_library());
		_liquid_mesher->reset();
	}

	next_phase();
}

void TerraTerrarinJob::phase_terrarin_mesh_setup() {
	if (should_return()) {
		return;
	}

	if (_mesher.is_valid()) {
		if (should_do()) {
			_mesher->add_chunk(_chunk);

			if (should_return()) {
				return;
			}
		}
	}

	if (_liquid_mesher.is_valid()) {
		_liquid_mesher->add_chunk(_chunk);
	}

	reset_stages();
	next_phase();
}

void TerraTerrarinJob::phase_collider() {
	Ref<TerraChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_CREATE_COLLIDER) == 0) {
		next_phase();
		return;
	}

	ERR_FAIL_COND(!_mesher.is_valid());

	if (should_return()) {
		return;
	}

	if (should_do()) {
		temp_arr_collider.append_array(_mesher->build_collider());

		if (should_return()) {
			return;
		}
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		if (_liquid_mesher.is_valid()) {
			temp_arr_collider_liquid.append_array(_liquid_mesher->build_collider());
		}
	}

	if (temp_arr_collider.size() == 0 && temp_arr_collider_liquid.size() == 0) {
		reset_stages();
		next_phase();
		next_phase();
		return;
	}

	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
	reset_stages();
	next_phase();
}

void TerraTerrarinJob::phase_physics_process() {
	Ref<TerraChunkDefault> chunk = _chunk;

	if (temp_arr_collider.size() != 0) {
		if (!chunk->meshes_has(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_BODY)) {
			chunk->colliders_create(TerraChunkDefault::MESH_INDEX_TERRARIN);
		}

		PhysicsServer::get_singleton()->shape_set_data(chunk->mesh_rid_get(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_SHAPE), temp_arr_collider);

		temp_arr_collider.resize(0);
	}

	if (temp_arr_collider_liquid.size() != 0) {
		if (Engine::get_singleton()->is_editor_hint()) {
			if (!chunk->meshes_has(TerraChunkDefault::MESH_INDEX_LIQUID, TerraChunkDefault::MESH_TYPE_INDEX_BODY)) {
				chunk->colliders_create(TerraChunkDefault::MESH_INDEX_LIQUID);
			}
		}
		/*
			else {
				if (!has_meshes(MESH_INDEX_LIQUID, MESH_TYPE_INDEX_AREA)) {
					create_colliders_area(MESH_INDEX_LIQUID);
				}
			}*/

		PhysicsServer::get_singleton()->shape_set_data(chunk->mesh_rid_get(TerraChunkDefault::MESH_INDEX_LIQUID, TerraChunkDefault::MESH_TYPE_INDEX_SHAPE), temp_arr_collider_liquid);

		temp_arr_collider_liquid.resize(0);
	}

	set_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
	reset_stages();
	next_phase();
}

void TerraTerrarinJob::phase_terrarin_mesh() {
	Ref<TerraChunkDefault> chunk = _chunk;

	ERR_FAIL_COND(!_mesher.is_valid());

	if (should_return()) {
		return;
	}

	if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		if (should_do()) {
			_mesher->bake_colors(_chunk);

			if (should_return()) {
				return;
			}
		}

		if (should_do()) {
			_liquid_mesher->bake_colors(_chunk);

			if (should_return()) {
				return;
			}
		}
	}

	if (_mesher->get_vertex_count() == 0 && (!_liquid_mesher.is_valid() || _liquid_mesher->get_vertex_count() == 0)) {
		reset_stages();
		next_phase();

		return;
	}

	if (_mesher->get_vertex_count() != 0) {
		if (should_do()) {
			temp_mesh_arr = _mesher->build_mesh();

			if (should_return()) {
				return;
			}
		}

		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (should_do()) {
			if (mesh_rid == RID()) {
				if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_CREATE_LODS) != 0)
					chunk->meshes_create(TerraChunkDefault::MESH_INDEX_TERRARIN, chunk->get_lod_num() + 1);
				else
					chunk->meshes_create(TerraChunkDefault::MESH_INDEX_TERRARIN, 1);

				mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 0);
			}

			if (VS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
#if !GODOT4
				VS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
#else
				VS::get_singleton()->mesh_clear(mesh_rid);
#endif

			if (should_return()) {
				return;
			}
		}

		if (should_do()) {
			VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

			if (chunk->get_library()->material_get(0).is_valid())
				VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, chunk->get_library()->material_get(0)->get_rid());

			if (should_return()) {
				return;
			}
		}


		if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_CREATE_LODS) != 0) {
			if (should_do()) {
				if (chunk->get_lod_num() >= 1) {
					//for lod 1 just remove uv2
					temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 1), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

					if (chunk->get_library()->material_get(1).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 1), 0, chunk->get_library()->material_get(1)->get_rid());
				}
				if (should_return()) {
					return;
				}
			}

			if (should_do()) {
				if (chunk->get_lod_num() >= 2) {
					Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);
					temp_mesh_arr = temp_mesh_arr2;

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 2), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr2);

					if (chunk->get_library()->material_get(2).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 2), 0, chunk->get_library()->material_get(2)->get_rid());
				}

				if (should_return()) {
					return;
				}
			}

			if (should_do()) {
				if (chunk->get_lod_num() >= 3) {
					Ref<ShaderMaterial> mat = chunk->get_library()->material_get(0);
					Ref<SpatialMaterial> spmat = chunk->get_library()->material_get(0);
					Ref<Texture> tex;

					if (mat.is_valid()) {
						tex = mat->get_shader_param("texture_albedo");
					} else if (spmat.is_valid()) {
						tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
					}

					if (tex.is_valid()) {
						temp_mesh_arr = bake_mesh_array_uv(temp_mesh_arr, tex);
						temp_mesh_arr[VisualServer::ARRAY_TEX_UV] = Variant();

						VisualServer::get_singleton()->mesh_add_surface_from_arrays(
								chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 3),
								VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

						if (chunk->get_library()->material_get(3).is_valid())
							VisualServer::get_singleton()->mesh_surface_set_material(
									chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 3), 0,
									chunk->get_library()->material_get(3)->get_rid());
					}
				}

				if (should_return()) {
					return;
				}
			}

#ifdef MESH_UTILS_PRESENT
			if (should_do()) {
				if (chunk->get_lod_num() > 4) {
					Ref<FastQuadraticMeshSimplifier> fqms;
					fqms.instance();
					fqms->set_preserve_border_edges(true);
					fqms->initialize(temp_mesh_arr);

					for (int i = 4; i < chunk->get_lod_num(); ++i) {
						fqms->simplify_mesh(temp_mesh_arr.size() * 0.8, 7);
						temp_mesh_arr = fqms->get_arrays();

						VisualServer::get_singleton()->mesh_add_surface_from_arrays(
								chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, i),
								VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

						if (chunk->get_library()->material_get(i).is_valid())
							VisualServer::get_singleton()->mesh_surface_set_material(
									chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, i), 0,
									chunk->get_library()->material_get(i)->get_rid());
					}
				}

				if (should_return()) {
					return;
				}
			}
#endif
		}
	}

	if (_liquid_mesher.is_valid() && _liquid_mesher->get_vertex_count() != 0) {
		if (should_do()) {
			temp_mesh_arr = _liquid_mesher->build_mesh();

			if (should_return()) {
				return;
			}
		}

		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_LIQUID, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (should_do()) {
			if (mesh_rid == RID()) {
				chunk->meshes_create(TerraChunkDefault::MESH_INDEX_LIQUID, 1);

				mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_LIQUID, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 0);
			}

			if (VS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
#if !GODOT4
				VS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
#else
				VS::get_singleton()->mesh_clear(mesh_rid);
#endif

			if (should_return()) {
				return;
			}
		}

		//	if (should_do()) {
		VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		if (chunk->get_library()->liquid_material_get(0).is_valid())
			VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, chunk->get_library()->liquid_material_get(0)->get_rid());

		//	if (should_return()) {
		//		return;
		//	}
		//}
	}

	if (has_meta("bptm_ulm")) {
		remove_meta("bptm_ulm");
	}

	if (has_meta("bptm_ullm")) {
		remove_meta("bptm_ullm");
	}

	if (has_meta("bptm_mm")) {
		remove_meta("bptm_mm");
	}

	if (has_meta("bptm_lmm")) {
		remove_meta("bptm_lmm");
	}

	reset_stages();
	next_phase();
}

void TerraTerrarinJob::phase_finalize() {
	set_complete(true); //So threadpool knows it's done

	next_job();
}

void TerraTerrarinJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<TerramanLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	if (_phase == 0) {
		phase_setup();
	} else if (_phase == 1) {
		phase_terrarin_mesh_setup();
	} else if (_phase == 2) {
		phase_collider();
	} else if (_phase == 4) {
		phase_terrarin_mesh();
	} else if (_phase == 5) {
		phase_finalize();
	} else if (_phase > 5) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		ERR_FAIL_MSG("TerraTerrarinJob: _phase is too high!");
	}
}

void TerraTerrarinJob::_reset() {
	TerraJob::_reset();

	_build_done = false;
	_phase = 0;

	ERR_FAIL_COND(!_mesher.is_valid());

	_mesher->set_voxel_scale(_chunk->get_voxel_scale());

	Ref<TerraChunkDefault> chunk = _chunk;
	Ref<TerraMesherDefault> md = _mesher;

	if (chunk.is_valid() && md.is_valid()) {
		md->set_build_flags(chunk->get_build_flags());
	}

	if (_liquid_mesher.is_valid()) {
		_liquid_mesher->set_voxel_scale(_chunk->get_voxel_scale());

		md = _liquid_mesher;

		if (chunk.is_valid() && md.is_valid()) {
			md->set_build_flags(chunk->get_build_flags());
		}
	}
}

void TerraTerrarinJob::_physics_process(float delta) {
	if (_phase == 3)
		phase_physics_process();
}

TerraTerrarinJob::TerraTerrarinJob() {
}

TerraTerrarinJob::~TerraTerrarinJob() {
	_mesher.unref();
	_liquid_mesher.unref();
}

void TerraTerrarinJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesher"), &TerraTerrarinJob::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "mesher"), &TerraTerrarinJob::set_mesher);

	ClassDB::bind_method(D_METHOD("get_liquid_mesher"), &TerraTerrarinJob::get_liquid_mesher);
	ClassDB::bind_method(D_METHOD("set_liquid_mesher", "mesher"), &TerraTerrarinJob::set_liquid_mesher);

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &TerraTerrarinJob::_physics_process);
}
