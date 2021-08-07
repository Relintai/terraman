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

#ifndef TERRA_PROP_JOB_H
#define TERRA_PROP_JOB_H

#include "terra_job.h"

#include "terra_mesher_job_step.h"

class TerraMesher;

class TerraPropJob : public TerraJob {
	GDCLASS(TerraPropJob, TerraJob);

public:
	Ref<TerraMesher> get_prop_mesher() const;
	void set_prop_mesher(const Ref<TerraMesher> &mesher);

	Ref<TerraMesherJobStep> get_jobs_step(const int index) const;
	void set_jobs_step(const int index, const Ref<TerraMesherJobStep> &step);
	void remove_jobs_step(const int index);
	void add_jobs_step(const Ref<TerraMesherJobStep> &step);
	int get_jobs_step_count() const;

	void phase_physics_process();
	void phase_prop();

	void _physics_process(float delta);
	void _execute_phase();
	void _reset();

	void phase_steps();

	void step_type_normal();
	void step_type_normal_lod();
	void step_type_drop_uv2();
	void step_type_merge_verts();
	void step_type_bake_texture();
	void step_type_simplify_mesh();

	TerraPropJob();
	~TerraPropJob();

protected:
	static void _bind_methods();

	Ref<TerraMesher> _prop_mesher;

	Vector<Ref<TerraMesherJobStep>> _job_steps;
	int _current_job_step;
	int _current_mesh;

	PoolVector<Vector3> temp_arr_collider;
	Array temp_mesh_arr;
};

#endif
