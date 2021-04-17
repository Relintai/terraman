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

#include "terra_mesher_job_step.h"

const String TerraMesherJobStep::BINDING_STRING_TERRA_TERRARIN_JOB_STEP_TYPE = "Normal,Drop UV2,Merge Verts,Bake Texture";

TerraMesherJobStep::TerraMesherJobStepType TerraMesherJobStep::get_job_type() const {
	return _job_type;
}
void TerraMesherJobStep::set_job_type(const TerraMesherJobStep::TerraMesherJobStepType value) {
	_job_type = value;
}

int TerraMesherJobStep::get_lod_index() const {
	return _lod_index;
}
void TerraMesherJobStep::set_lod_index(const int value) {
	_lod_index = value;
}

TerraMesherJobStep::TerraMesherJobStep() {
	_job_type = TYPE_NORMAL;
	_lod_index = 0;
}

TerraMesherJobStep::~TerraMesherJobStep() {
}

void TerraMesherJobStep::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_job_type"), &TerraMesherJobStep::get_job_type);
	ClassDB::bind_method(D_METHOD("set_job_type", "value"), &TerraMesherJobStep::set_job_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "job_type", PROPERTY_HINT_ENUM, TerraMesherJobStep::BINDING_STRING_TERRA_TERRARIN_JOB_STEP_TYPE), "set_job_type", "get_job_type");

	ClassDB::bind_method(D_METHOD("get_lod_index"), &TerraMesherJobStep::get_lod_index);
	ClassDB::bind_method(D_METHOD("set_lod_index", "value"), &TerraMesherJobStep::set_lod_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_index"), "set_lod_index", "get_lod_index");

	BIND_ENUM_CONSTANT(TYPE_NORMAL);
	BIND_ENUM_CONSTANT(TYPE_DROP_UV2);
	BIND_ENUM_CONSTANT(TYPE_MERGE_VERTS);
	BIND_ENUM_CONSTANT(TYPE_BAKE_TEXTURE);
}
