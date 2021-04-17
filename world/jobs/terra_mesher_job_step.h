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

#ifndef TERRA_TERRARIN_JOB_STEP_H
#define TERRA_TERRARIN_JOB_STEP_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/reference.h"
#else
#include "core/reference.h"
#endif

#ifdef MESH_UTILS_PRESENT
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

class TerraMesherJobStep : public Reference {
	GDCLASS(TerraMesherJobStep, Reference);

public:
	enum TerraMesherJobStepType {
		TYPE_NORMAL = 0,
		TYPE_DROP_UV2,
		TYPE_MERGE_VERTS,
		TYPE_BAKE_TEXTURE,
		TYPE_SIMPLIFY_MESH,
		TYPE_OTHER,
	};

	static const String BINDING_STRING_TERRA_TERRARIN_JOB_STEP_TYPE;

	TerraMesherJobStepType get_job_type() const;
	void set_job_type(const TerraMesherJobStepType value);

	int get_lod_index() const;
	void set_lod_index(const int value);

	#ifdef MESH_UTILS_PRESENT
		Ref<FastQuadraticMeshSimplifier> get_fqms();
		void set_fqms(const Ref<FastQuadraticMeshSimplifier> &val);

		float get_simplification_step_ratio() const;
		void set_simplification_step_ratio(const float value);

		int get_simplification_steps() const;
		void set_simplification_steps(const float value);
	#endif

	TerraMesherJobStep();
	~TerraMesherJobStep();

protected:
	static void _bind_methods();

	TerraMesherJobStepType _job_type;
	int _lod_index;

	#ifdef MESH_UTILS_PRESENT
		Ref<FastQuadraticMeshSimplifier> _fqms;
		float _simplification_step_ratio;
		int _simplification_steps;
	#endif
};

VARIANT_ENUM_CAST(TerraMesherJobStep::TerraMesherJobStepType);

#endif
