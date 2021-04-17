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

class TerraTerrarinJobStep : public Reference {
	GDCLASS(TerraTerrarinJobStep, Reference);

public:
	enum TerraTerrarinJobStepType {
		TYPE_NORMAL = 0,
		TYPE_DROP_UV2,
		TYPE_MERGE_VERTS,
		TYPE_BAKE_TEXTURE,
	};

	static const String BINDING_STRING_TERRA_TERRARIN_JOB_STEP_TYPE;

	TerraTerrarinJobStepType get_job_type() const;
	void set_job_type(const TerraTerrarinJobStepType value);

	int get_lod_index() const;
	void set_lod_index(const int value);

	TerraTerrarinJobStep();
	~TerraTerrarinJobStep();

protected:
	static void _bind_methods();

	TerraTerrarinJobStepType _job_type;
	int _lod_index;
};

VARIANT_ENUM_CAST(TerraTerrarinJobStep::TerraTerrarinJobStepType);

#endif
