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

#include "terra_material_cache.h"

//Materials
Ref<Material> TerraMaterialCache::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

Ref<Material> TerraMaterialCache::material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void TerraMaterialCache::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void TerraMaterialCache::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void TerraMaterialCache::material_remove(const int index) {
	_materials.remove(index);
}

int TerraMaterialCache::material_get_num() const {
	return _materials.size();
}

void TerraMaterialCache::materials_clear() {
	_materials.clear();
}

Vector<Variant> TerraMaterialCache::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void TerraMaterialCache::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Surfaces
Ref<TerraSurface> TerraMaterialCache::voxel_surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _surfaces.size(), Ref<TerraSurface>());

	return _surfaces[index];
}
Ref<TerraSurface> TerraMaterialCache::voxel_surface_id_get(const int id) {
	Ref<TerraSurface> surface;

	for (int i = 0; i < _surfaces.size(); ++i) {
		surface = _surfaces[i];

		if (surface.is_valid()) {
			if (surface->get_id() == id) {
				break;
			}
		}
	}

	return surface;
}
void TerraMaterialCache::voxel_surface_add(Ref<TerraSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	_surfaces.push_back(value);
}
void TerraMaterialCache::voxel_surface_set(int index, Ref<TerraSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_surfaces.size() < index) {
		_surfaces.resize(index + 1);
	}

	_surfaces.set(index, value);
}
void TerraMaterialCache::voxel_surface_remove(const int index) {
	_surfaces.remove(index);
}
int TerraMaterialCache::voxel_surface_get_num() const {
	return _surfaces.size();
}
void TerraMaterialCache::voxel_surfaces_clear() {
	_surfaces.clear();
}

void TerraMaterialCache::refresh_rects() {
}

void TerraMaterialCache::setup_material_albedo(Ref<Texture> texture) {
	if (has_method("_setup_material_albedo"))
		call("_setup_material_albedo", texture);
}

TerraMaterialCache::TerraMaterialCache() {
	material_users = 0;
}

TerraMaterialCache::~TerraMaterialCache() {
	_materials.clear();
	_surfaces.clear();
}

void TerraMaterialCache::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));

	ClassDB::bind_method(D_METHOD("material_get", "index"), &TerraMaterialCache::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &TerraMaterialCache::material_lod_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &TerraMaterialCache::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &TerraMaterialCache::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &TerraMaterialCache::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &TerraMaterialCache::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &TerraMaterialCache::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &TerraMaterialCache::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &TerraMaterialCache::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	ClassDB::bind_method(D_METHOD("voxel_surface_get", "index"), &TerraMaterialCache::voxel_surface_get);
	ClassDB::bind_method(D_METHOD("voxel_surface_id_get", "index"), &TerraMaterialCache::voxel_surface_id_get);

	ClassDB::bind_method(D_METHOD("voxel_surface_add", "value"), &TerraMaterialCache::voxel_surface_add);
	ClassDB::bind_method(D_METHOD("voxel_surface_set", "index", "surface"), &TerraMaterialCache::voxel_surface_set);
	ClassDB::bind_method(D_METHOD("voxel_surface_remove", "index"), &TerraMaterialCache::voxel_surface_remove);
	ClassDB::bind_method(D_METHOD("voxel_surface_get_num"), &TerraMaterialCache::voxel_surface_get_num);
	ClassDB::bind_method(D_METHOD("voxel_surfaces_clear"), &TerraMaterialCache::voxel_surfaces_clear);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &TerraMaterialCache::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "texture"), &TerraMaterialCache::setup_material_albedo);
}
