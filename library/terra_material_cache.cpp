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

bool TerraMaterialCache::get_initialized() {
	return _initialized;
}
void TerraMaterialCache::set_initialized(const bool value) {
	_initialized = value;
}

int TerraMaterialCache::get_ref_count() {
	return _ref_count;
}
void TerraMaterialCache::set_ref_count(const int value) {
	_ref_count = value;
}
void TerraMaterialCache::inc_ref_count() {
	_ref_count += 1;
}
void TerraMaterialCache::dec_ref_count() {
	_ref_count -= 1;
}

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
Ref<TerraSurface> TerraMaterialCache::surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _surfaces.size(), Ref<TerraSurface>());

	return _surfaces[index];
}
Ref<TerraSurface> TerraMaterialCache::surface_id_get(const int id) {
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
void TerraMaterialCache::surface_add(Ref<TerraSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	_surfaces.push_back(value);
}
void TerraMaterialCache::surface_set(int index, Ref<TerraSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_surfaces.size() < index) {
		_surfaces.resize(index + 1);
	}

	_surfaces.set(index, value);
}
void TerraMaterialCache::surface_remove(const int index) {
	_surfaces.remove(index);
}
int TerraMaterialCache::surface_get_num() const {
	return _surfaces.size();
}
void TerraMaterialCache::surfaces_clear() {
	_surfaces.clear();
}

void TerraMaterialCache::additional_texture_add(const Ref<Texture> &tex) {
	_additional_textures.push_back(tex);
}
void TerraMaterialCache::additional_texture_remove(const Ref<Texture> &tex) {
	for (int i = 0; i < _additional_textures.size(); ++i) {
		if (_additional_textures[i] == tex) {
			_additional_textures.remove(i);
			return;
		}
	}
}
void TerraMaterialCache::additional_texture_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _additional_textures.size());

	_additional_textures.remove(index);
}
void TerraMaterialCache::additional_textures_clear() {
	_additional_textures.clear();
}
int TerraMaterialCache::additional_texture_count() {
	return _additional_textures.size();
}
Ref<Texture> TerraMaterialCache::additional_texture_get(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<Texture>());

	return _additional_textures[index];
}
Ref<AtlasTexture> TerraMaterialCache::additional_texture_get_atlas(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<AtlasTexture>());

	return Ref<AtlasTexture>();
}
Rect2 TerraMaterialCache::additional_texture_get_uv_rect(const Ref<Texture> &tex) {
	return Rect2(0, 0, 1, 1);
}

void TerraMaterialCache::refresh_rects() {
	_initialized = true;
}

void TerraMaterialCache::setup_material_albedo(Ref<Texture> texture) {
	if (has_method("_setup_material_albedo"))
		call("_setup_material_albedo", texture);
}

TerraMaterialCache::TerraMaterialCache() {
	_ref_count = 0;
	_initialized = false;
}

TerraMaterialCache::~TerraMaterialCache() {
	_materials.clear();
	_surfaces.clear();
}

void TerraMaterialCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &TerraMaterialCache::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &TerraMaterialCache::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized"), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("get_ref_count"), &TerraMaterialCache::get_ref_count);
	ClassDB::bind_method(D_METHOD("set_ref_count", "value"), &TerraMaterialCache::set_ref_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mat_ref_count"), "set_ref_count", "get_ref_count");
	ClassDB::bind_method(D_METHOD("inc_ref_count"), &TerraMaterialCache::inc_ref_count);
	ClassDB::bind_method(D_METHOD("dec_ref_count"), &TerraMaterialCache::dec_ref_count);

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

	ClassDB::bind_method(D_METHOD("surface_get", "index"), &TerraMaterialCache::surface_get);
	ClassDB::bind_method(D_METHOD("surface_id_get", "index"), &TerraMaterialCache::surface_id_get);

	ClassDB::bind_method(D_METHOD("surface_add", "value"), &TerraMaterialCache::surface_add);
	ClassDB::bind_method(D_METHOD("surface_set", "index", "surface"), &TerraMaterialCache::surface_set);
	ClassDB::bind_method(D_METHOD("surface_remove", "index"), &TerraMaterialCache::surface_remove);
	ClassDB::bind_method(D_METHOD("surface_get_num"), &TerraMaterialCache::surface_get_num);
	ClassDB::bind_method(D_METHOD("surfaces_clear"), &TerraMaterialCache::surfaces_clear);

	ClassDB::bind_method(D_METHOD("additional_texture_add", "tex"), &TerraMaterialCache::additional_texture_add);
	ClassDB::bind_method(D_METHOD("additional_texture_remove", "tex"), &TerraMaterialCache::additional_texture_remove);
	ClassDB::bind_method(D_METHOD("additional_texture_remove_index", "index"), &TerraMaterialCache::additional_texture_remove_index);
	ClassDB::bind_method(D_METHOD("additional_textures_clear"), &TerraMaterialCache::additional_textures_clear);
	ClassDB::bind_method(D_METHOD("additional_texture_count"), &TerraMaterialCache::additional_texture_count);
	ClassDB::bind_method(D_METHOD("additional_texture_get", "index"), &TerraMaterialCache::additional_texture_get);
	ClassDB::bind_method(D_METHOD("additional_texture_get_atlas", "index"), &TerraMaterialCache::additional_texture_get_atlas);
	ClassDB::bind_method(D_METHOD("additional_texture_get_uv_rect", "tex"), &TerraMaterialCache::additional_texture_get_uv_rect);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &TerraMaterialCache::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "texture"), &TerraMaterialCache::setup_material_albedo);
}
