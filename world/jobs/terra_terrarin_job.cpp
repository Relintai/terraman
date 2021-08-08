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

#include "../../library/terra_material_cache.h"
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

Ref<TerraMesherJobStep> TerraTerrarinJob::get_jobs_step(int index) const {
	ERR_FAIL_INDEX_V(index, _job_steps.size(), Ref<TerraMesherJobStep>());

	return _job_steps.get(index);
}
void TerraTerrarinJob::set_jobs_step(int index, const Ref<TerraMesherJobStep> &step) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.set(index, step);
}
void TerraTerrarinJob::remove_jobs_step(const int index) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.remove(index);
}
void TerraTerrarinJob::add_jobs_step(const Ref<TerraMesherJobStep> &step) {
	_job_steps.push_back(step);
}
int TerraTerrarinJob::get_jobs_step_count() const {
	return _job_steps.size();
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

void TerraTerrarinJob::phase_library_setup() {
	if (should_return()) {
		return;
	}

	Ref<TerramanLibrary> lib = _chunk->get_library();

	if (!lib.is_valid()) {
		next_phase();
		return;
	}

	if (lib->supports_caching()) {
		if (!_chunk->material_cache_key_has()) {
			lib->material_cache_get_key(_chunk);
		} else {
			Ref<TerraMaterialCache> cache = lib->material_cache_get(_chunk->material_cache_key_get());

			if (!cache.is_valid()) {
				next_phase();
				return;
			}

			//Note: without threadpool and threading none of this can happen, as cache will get initialized the first time a thread requests it!
			while (!cache->get_initialized()) {
				//Means it's currently merging the atlases on a different thread.
				//Let's just wait
				OS::get_singleton()->delay_usec(100);
			}
		}
	}

	next_phase();

	if (should_return()) {
		return;
	}
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
		//if (should_do()) {
		//	_mesher->bake_colors(_chunk);

		//	if (should_return()) {
		//		return;
		//	}
		//}

		if (should_do()) {
			if (_liquid_mesher.is_valid()) {
				_liquid_mesher->bake_colors(_chunk);

				if (should_return()) {
					return;
				}
			}
		}
	}

	if (_mesher->get_vertex_count() == 0 && (!_liquid_mesher.is_valid() || _liquid_mesher->get_vertex_count() == 0)) {
		reset_stages();
		next_phase();

		return;
	}

	//set up the meshes
	if (should_do()) {
		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (mesh_rid == RID()) {
			//need to allocate the meshes

			//first count how many we need
			int count = 0;
			for (int i = 0; i < _job_steps.size(); ++i) {
				Ref<TerraMesherJobStep> step = _job_steps[i];

				ERR_FAIL_COND(!step.is_valid());

				switch (step->get_job_type()) {
					case TerraMesherJobStep::TYPE_NORMAL:
						++count;
						break;
					case TerraMesherJobStep::TYPE_NORMAL_LOD:
						++count;
						break;
					case TerraMesherJobStep::TYPE_DROP_UV2:
						++count;
						break;
					case TerraMesherJobStep::TYPE_MERGE_VERTS:
						++count;
						break;
					case TerraMesherJobStep::TYPE_BAKE_TEXTURE:
						++count;
						break;
					case TerraMesherJobStep::TYPE_SIMPLIFY_MESH:
#ifdef MESH_UTILS_PRESENT
						count += step->get_simplification_steps();
#endif
						break;
					default:
						break;
				}
			}

			//allocate
			if (count > 0)
				chunk->meshes_create(TerraChunkDefault::MESH_INDEX_TERRARIN, count);

		} else {
			//we have the meshes, just clear
			int count = chunk->mesh_rid_get_count(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH);

			for (int i = 0; i < count; ++i) {
				mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, i);

				if (VS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
#if !GODOT4
					VS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
#else
					VS::get_singleton()->mesh_clear(mesh_rid);
#endif
			}
		}
	}

	for (; _current_job_step < _job_steps.size();) {
		Ref<TerraMesherJobStep> step = _job_steps[_current_job_step];

		ERR_FAIL_COND(!step.is_valid());

		switch (step->get_job_type()) {
			case TerraMesherJobStep::TYPE_NORMAL:
				step_type_normal();
				break;
			case TerraMesherJobStep::TYPE_NORMAL_LOD:
				step_type_normal_lod();
				break;
			case TerraMesherJobStep::TYPE_DROP_UV2:
				step_type_drop_uv2();
				break;
			case TerraMesherJobStep::TYPE_MERGE_VERTS:
				step_type_merge_verts();
				break;
			case TerraMesherJobStep::TYPE_BAKE_TEXTURE:
				step_type_bake_texture();
				break;
			case TerraMesherJobStep::TYPE_SIMPLIFY_MESH:
				step_type_simplify_mesh();
				break;
			case TerraMesherJobStep::TYPE_OTHER:
				//do nothing
				break;
		}

		++_current_job_step;

		if (should_return()) {
			return;
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

		if (chunk->get_library()->liquid_material_lod_get(0).is_valid())
			VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, chunk->get_library()->liquid_material_lod_get(0)->get_rid());

		//	if (should_return()) {
		//		return;
		//	}
		//}
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
		phase_library_setup();
	} else if (_phase == 2) {
		phase_terrarin_mesh_setup();
	} else if (_phase == 3) {
		phase_collider();
	} else if (_phase == 5) {
		phase_terrarin_mesh();
	} else if (_phase == 6) {
		phase_finalize();
	} else if (_phase > 6) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		ERR_FAIL_MSG("TerraTerrarinJob: _phase is too high!");
	}
}

void TerraTerrarinJob::_reset() {
	TerraJob::_reset();

	_build_done = false;
	_phase = 0;

	_current_job_step = 0;
	_current_mesh = 0;

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
	if (_phase == 4)
		phase_physics_process();
}

void TerraTerrarinJob::step_type_normal() {
	Ref<TerraChunkDefault> chunk = _chunk;

	//TODO make this automatic in build_mesh
	if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		_mesher->bake_colors(_chunk);
	}

	temp_mesh_arr = _mesher->build_mesh();

	RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerraTerrarinJob::step_type_normal_lod() {
	Ref<TerraMesherJobStep> step = _job_steps[_current_job_step];

	ERR_FAIL_COND(!step.is_valid());

	Ref<TerraChunkDefault> chunk = _chunk;

	_mesher->set_lod_index(step->get_lod_index());
	_mesher->reset();
	_mesher->add_chunk(_chunk);

	//TODO make this automatic in build_mesh
	if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		_mesher->bake_colors(_chunk);
	}

	temp_mesh_arr = _mesher->build_mesh();

	RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerraTerrarinJob::step_type_drop_uv2() {
	Ref<TerraChunkDefault> chunk = _chunk;

	RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

	VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerraTerrarinJob::step_type_merge_verts() {
	Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);
	temp_mesh_arr = temp_mesh_arr2;

	Ref<TerraChunkDefault> chunk = _chunk;
	RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerraTerrarinJob::step_type_bake_texture() {
	Ref<TerraChunkDefault> chunk = _chunk;

	Ref<ShaderMaterial> mat = chunk->get_library()->material_lod_get(0);
	Ref<SpatialMaterial> spmat = chunk->get_library()->material_lod_get(0);
	Ref<Texture> tex;

	if (mat.is_valid()) {
		tex = mat->get_shader_param("texture_albedo");
	} else if (spmat.is_valid()) {
		tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
	}

	if (tex.is_valid()) {
		temp_mesh_arr = bake_mesh_array_uv(temp_mesh_arr, tex);
		temp_mesh_arr[VisualServer::ARRAY_TEX_UV] = Variant();

		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		Ref<Material> lmat;

		if (chunk->material_cache_key_has()) {
			lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
		} else {
			lmat = chunk->get_library()->material_lod_get(_current_mesh);
		}

		if (lmat.is_valid()) {
			VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}
	}

	++_current_mesh;
}

void TerraTerrarinJob::step_type_simplify_mesh() {
#ifdef MESH_UTILS_PRESENT

	Ref<TerraChunkDefault> chunk = _chunk;
	Ref<TerraMesherJobStep> step = _job_steps[_current_job_step];
	ERR_FAIL_COND(!step.is_valid());
	Ref<FastQuadraticMeshSimplifier> fqms = step->get_fqms();
	ERR_FAIL_COND(!fqms.is_valid());

	fqms->initialize(temp_mesh_arr);

	for (int i = 0; i < step->get_simplification_steps(); ++i) {
		fqms->simplify_mesh(temp_mesh_arr.size() * step->get_simplification_step_ratio(), step->get_simplification_agressiveness());
		temp_mesh_arr = fqms->get_arrays();

		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_TERRARIN, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		Ref<Material> lmat;

		if (chunk->material_cache_key_has()) {
			lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
		} else {
			lmat = chunk->get_library()->material_lod_get(_current_mesh);
		}

		if (lmat.is_valid()) {
			VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}

		++_current_mesh;
	}

#endif
}

TerraTerrarinJob::TerraTerrarinJob() {
	_current_job_step = 0;
	_current_mesh = 0;
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

	ClassDB::bind_method(D_METHOD("get_jobs_step", "index"), &TerraTerrarinJob::get_jobs_step);
	ClassDB::bind_method(D_METHOD("set_jobs_step", "index", "mesher"), &TerraTerrarinJob::set_jobs_step);
	ClassDB::bind_method(D_METHOD("remove_jobs_step", "index"), &TerraTerrarinJob::remove_jobs_step);
	ClassDB::bind_method(D_METHOD("add_jobs_step", "mesher"), &TerraTerrarinJob::add_jobs_step);
	ClassDB::bind_method(D_METHOD("get_jobs_step_count"), &TerraTerrarinJob::get_jobs_step_count);

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &TerraTerrarinJob::_physics_process);
}
