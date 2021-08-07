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

#include "terra_prop_job.h"

#include "../../defines.h"

#include "../../library/terra_material_cache.h"
#include "../../library/terra_surface.h"
#include "../../library/terraman_library.h"

#include "../../meshers/default/terra_mesher_default.h"
#include "../../meshers/terra_mesher.h"
#include "../default/terra_chunk_default.h"

#ifdef MESH_DATA_RESOURCE_PRESENT
#include "../../../mesh_data_resource/mesh_data_resource.h"
#endif

#include "../../world/default/terra_world_default.h"

#ifdef MESH_UTILS_PRESENT
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

Ref<TerraMesher> TerraPropJob::get_prop_mesher() const {
	return _prop_mesher;
}
void TerraPropJob::set_prop_mesher(const Ref<TerraMesher> &mesher) {
	_prop_mesher = mesher;
}

Ref<TerraMesherJobStep> TerraPropJob::get_jobs_step(int index) const {
	ERR_FAIL_INDEX_V(index, _job_steps.size(), Ref<TerraMesherJobStep>());

	return _job_steps.get(index);
}
void TerraPropJob::set_jobs_step(int index, const Ref<TerraMesherJobStep> &step) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.set(index, step);
}
void TerraPropJob::remove_jobs_step(const int index) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.remove(index);
}
void TerraPropJob::add_jobs_step(const Ref<TerraMesherJobStep> &step) {
	_job_steps.push_back(step);
}
int TerraPropJob::get_jobs_step_count() const {
	return _job_steps.size();
}

void TerraPropJob::phase_physics_process() {
	Ref<TerraChunkDefault> chunk = _chunk;

	//TODO this should only update the differences
	for (int i = 0; i < chunk->collider_get_count(); ++i) {
		PhysicsServer::get_singleton()->free(chunk->collider_get_body(i));
	}

	chunk->colliders_clear();

#ifdef MESH_DATA_RESOURCE_PRESENT
	for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
		Ref<MeshDataResource> mdr = chunk->mesh_data_resource_get(i);

		for (int j = 0; j < mdr->get_collision_shape_count(); ++j) {
			Ref<Shape> shape = mdr->get_collision_shape(j);
			Transform offset = mdr->get_collision_shape_offset(j);

			if (!shape.is_valid()) {
				continue;
			}

			RID body = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

			Transform transform = chunk->mesh_data_resource_get_transform(i);
			transform *= offset;

			PhysicsServer::get_singleton()->body_add_shape(body, shape->get_rid());

			//TODO store the layer mask somewhere
			PhysicsServer::get_singleton()->body_set_collision_layer(body, 1);
			PhysicsServer::get_singleton()->body_set_collision_mask(body, 1);

			if (chunk->get_voxel_world()->is_inside_tree() && chunk->get_voxel_world()->is_inside_world()) {
				Ref<World> world = chunk->get_voxel_world()->GET_WORLD();

				if (world.is_valid() && world->get_space() != RID()) {
					PhysicsServer::get_singleton()->body_set_space(body, world->get_space());
				}
			}

			PhysicsServer::get_singleton()->body_set_state(body, PhysicsServer::BODY_STATE_TRANSFORM, chunk->get_transform() * transform);

			chunk->collider_add(transform, shape, shape->get_rid(), body);
		}
	}
#endif

#if TOOLS_ENABLED
	if (SceneTree::get_singleton()->is_debugging_collisions_hint() && chunk->collider_get_count() > 0) {
		chunk->draw_debug_mdr_colliders();
	}
#endif

	set_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
	next_phase();
}

void TerraPropJob::phase_prop() {
#ifdef MESH_DATA_RESOURCE_PRESENT
	Ref<TerraChunkDefault> chunk = _chunk;

	if (!get_prop_mesher().is_valid()) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		return;
	}

	if (should_do()) {
		if (chunk->mesh_data_resource_get_count() == 0) {
			reset_stages();
			set_complete(true); //So threadpool knows it's done
			next_job();
			return;
		}

		for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
			if (chunk->mesh_data_resource_get_is_inside(i)) {
				get_prop_mesher()->add_mesh_data_resource_transform(chunk->mesh_data_resource_get(i), chunk->mesh_data_resource_get_transform(i), chunk->mesh_data_resource_get_uv_rect(i));
			}
		}

		if (get_prop_mesher()->get_vertex_count() == 0) {
			reset_stages();

			set_complete(true); //So threadpool knows it's done
			next_job();
			return;
		}

		if (should_return()) {
			return;
		}
	}

	if (should_do()) {
		if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			get_prop_mesher()->bake_colors(_chunk);
		}

		if (should_return()) {
			return;
		}
	}

	if (should_do()) {
		if ((chunk->get_build_flags() & TerraChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			TerraWorldDefault *world = Object::cast_to<TerraWorldDefault>(chunk->get_voxel_world());

			if (world) {
				for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
					if (!chunk->mesh_data_resource_get_is_inside(i)) {
						Ref<MeshDataResource> mdr = chunk->mesh_data_resource_get(i);

						ERR_CONTINUE(!mdr.is_valid());

						Transform trf = chunk->mesh_data_resource_get_transform(i);

						Array arr = mdr->get_array();

						if (arr.size() <= Mesh::ARRAY_VERTEX) {
							continue;
						}

						PoolVector3Array varr = arr[Mesh::ARRAY_VERTEX];

						if (varr.size() == 0) {
							continue;
						}

						PoolColorArray carr = world->get_vertex_colors(trf, varr);

						get_prop_mesher()->add_mesh_data_resource_transform_colored(mdr, trf, carr, chunk->mesh_data_resource_get_uv_rect(i));
					}
				}
			}
		}
	}

#endif

	reset_stages();
	next_phase();
}

void TerraPropJob::_physics_process(float delta) {
	if (_phase == 0)
		phase_physics_process();
}

void TerraPropJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<TerramanLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	Ref<TerraChunkDefault> chunk = _chunk;

	if (!chunk.is_valid()
#ifdef MESH_DATA_RESOURCE_PRESENT
			|| chunk->mesh_data_resource_get_count() == 0
#endif
	) {
		set_complete(true);
		next_job();
		return;
	}

	if (_phase == 1) {
		phase_prop();
	} else if (_phase == 2) {
		phase_steps();
	} else if (_phase > 2) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		ERR_FAIL_MSG("TerraPropJob: _phase is too high!");
	}
}

void TerraPropJob::_reset() {
	TerraJob::_reset();

	_build_done = false;
	_phase = 0;

	_current_job_step = 0;
	_current_mesh = 0;

	if (get_prop_mesher().is_valid()) {
		get_prop_mesher()->reset();
		get_prop_mesher()->set_library(_chunk->get_library());
	}

	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
}

void TerraPropJob::phase_steps() {
	Ref<TerraChunkDefault> chunk = _chunk;

	ERR_FAIL_COND(!_prop_mesher.is_valid());

	if (should_return()) {
		return;
	}

	if (_prop_mesher->get_vertex_count() == 0) {
		reset_stages();
		//next_phase();
		set_complete(true); //So threadpool knows it's done
		next_job();

		return;
	}

	//set up the meshes
	if (should_do()) {
		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH, 0);

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
				chunk->meshes_create(TerraChunkDefault::MESH_INDEX_PROP, count);

		} else {
			//we have the meshes, just clear
			int count = chunk->mesh_rid_get_count(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH);

			for (int i = 0; i < count; ++i) {
				mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH, i);

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

	reset_stages();
	//next_phase();

	set_complete(true); //So threadpool knows it's done
	next_job();
}

void TerraPropJob::step_type_normal() {
	//TODO add a lighting generation step

	Ref<TerraChunkDefault> chunk = _chunk;

	temp_mesh_arr = _prop_mesher->build_mesh();

	RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->prop_material_cache_key_has()) {
		lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerraPropJob::step_type_normal_lod() {
	print_error("Error: step_type_normal_lod doesn't work for TerraPropJobs!");

	++_current_mesh;
}

void TerraPropJob::step_type_drop_uv2() {
	Ref<TerraChunkDefault> chunk = _chunk;

	RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

	VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->prop_material_cache_key_has()) {
		lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerraPropJob::step_type_merge_verts() {
	Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);
	temp_mesh_arr = temp_mesh_arr2;

	Ref<TerraChunkDefault> chunk = _chunk;
	RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->prop_material_cache_key_has()) {
		lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void TerraPropJob::step_type_bake_texture() {
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

		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		Ref<Material> lmat;

		if (chunk->prop_material_cache_key_has()) {
			lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
		} else {
			lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
		}

		if (lmat.is_valid()) {
			VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}
	}

	++_current_mesh;
}

void TerraPropJob::step_type_simplify_mesh() {
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

		RID mesh_rid = chunk->mesh_rid_get_index(TerraChunkDefault::MESH_INDEX_PROP, TerraChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		Ref<Material> lmat;

		if (chunk->prop_material_cache_key_has()) {
			lmat = chunk->get_library()->prop_material_cache_get(_chunk->prop_material_cache_key_get())->material_lod_get(_current_mesh);
		} else {
			lmat = chunk->get_library()->prop_material_lod_get(_current_mesh);
		}

		if (lmat.is_valid()) {
			VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}

		++_current_mesh;
	}

#endif
}

TerraPropJob::TerraPropJob() {
	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);

	_current_job_step = 0;
	_current_mesh = 0;
}

TerraPropJob::~TerraPropJob() {
}

void TerraPropJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop_mesher"), &TerraPropJob::get_prop_mesher);
	ClassDB::bind_method(D_METHOD("set_prop_mesher", "mesher"), &TerraPropJob::set_prop_mesher);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_mesher", PROPERTY_HINT_RESOURCE_TYPE, "TerraMesher", 0), "set_prop_mesher", "get_prop_mesher");

	ClassDB::bind_method(D_METHOD("get_jobs_step", "index"), &TerraPropJob::get_jobs_step);
	ClassDB::bind_method(D_METHOD("set_jobs_step", "index", "mesher"), &TerraPropJob::set_jobs_step);
	ClassDB::bind_method(D_METHOD("remove_jobs_step", "index"), &TerraPropJob::remove_jobs_step);
	ClassDB::bind_method(D_METHOD("add_jobs_step", "mesher"), &TerraPropJob::add_jobs_step);
	ClassDB::bind_method(D_METHOD("get_jobs_step_count"), &TerraPropJob::get_jobs_step_count);

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &TerraPropJob::_physics_process);
}
