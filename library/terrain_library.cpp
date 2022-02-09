/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "terrain_library.h"

#include "../world/terrain_chunk.h"

#include "terrain_material_cache.h"

#ifdef PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

#include "scene/resources/packed_scene.h"

#include "../defines.h"

#if VERSION_MAJOR > 3

#define TMCGDVIRTUAL1(m_class, m_name, m_type1)                                                                                                                                                                 \
	bool m_class::_gdvirtual_##m_name##_call(m_type1 arg1) {                                                                                                                                                    \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			Callable::CallError ce;                                                                                                                                                                             \
			Variant vargs[1] = { Variant(arg1) };                                                                                                                                                               \
			const Variant *vargptrs[1] = { &vargs[0] };                                                                                                                                                         \
                                                                                                                                                                                                                \
			script_instance->call(_gdvirtual_##m_name##_sn, (const Variant **)vargptrs, 1, ce);                                                                                                                 \
			if (ce.error == Callable::CallError::CALL_OK) {                                                                                                                                                     \
				return true;                                                                                                                                                                                    \
			}                                                                                                                                                                                                   \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			PtrToArg<m_type1>::EncodeT argval1 = arg1;                                                                                                                                                          \
			const GDNativeTypePtr argptrs[1] = { &argval1 };                                                                                                                                                    \
                                                                                                                                                                                                                \
			_gdvirtual_##m_name(_get_extension_instance(), (const GDNativeTypePtr *)argptrs, nullptr);                                                                                                          \
                                                                                                                                                                                                                \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
                                                                                                                                                                                                                \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
	bool m_class::_gdvirtual_##m_name##_overridden() const {                                                                                                                                                    \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			return script_instance->has_method(_gdvirtual_##m_name##_sn);                                                                                                                                       \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
                                                                                                                                                                                                                \
	MethodInfo m_class::_gdvirtual_##m_name##_get_method_info() {                                                                                                                                               \
		MethodInfo method_info;                                                                                                                                                                                 \
		method_info.name = #m_name;                                                                                                                                                                             \
		method_info.flags = METHOD_FLAG_VIRTUAL;                                                                                                                                                                \
		method_info.arguments.push_back(GetTypeInfo<m_type1>::get_class_info());                                                                                                                                \
                                                                                                                                                                                                                \
		return method_info;                                                                                                                                                                                     \
	}

#define TMCGDVIRTUAL1R(m_class, m_ret, m_name, m_type1)                                                                                                                                                         \
	bool m_class::_gdvirtual_##m_name##_call(m_type1 arg1, m_ret &r_ret) {                                                                                                                                      \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			Callable::CallError ce;                                                                                                                                                                             \
			Variant vargs[1] = { Variant(arg1) };                                                                                                                                                               \
			const Variant *vargptrs[1] = { &vargs[0] };                                                                                                                                                         \
                                                                                                                                                                                                                \
			Variant ret = script_instance->call(_gdvirtual_##m_name##_sn, (const Variant **)vargptrs, 1, ce);                                                                                                   \
			if (ce.error == Callable::CallError::CALL_OK) {                                                                                                                                                     \
				r_ret = VariantCaster<m_ret>::cast(ret);                                                                                                                                                        \
				return true;                                                                                                                                                                                    \
			}                                                                                                                                                                                                   \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			PtrToArg<m_type1>::EncodeT argval1 = arg1;                                                                                                                                                          \
			const GDNativeTypePtr argptrs[1] = { &argval1 };                                                                                                                                                    \
                                                                                                                                                                                                                \
			PtrToArg<m_ret>::EncodeT ret;                                                                                                                                                                       \
			_gdvirtual_##m_name(_get_extension_instance(), (const GDNativeTypePtr *)argptrs, &ret);                                                                                                             \
			r_ret = (m_ret)ret;                                                                                                                                                                                 \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
                                                                                                                                                                                                                \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
	bool m_class::_gdvirtual_##m_name##_overridden() const {                                                                                                                                                    \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			return script_instance->has_method(_gdvirtual_##m_name##_sn);                                                                                                                                       \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
                                                                                                                                                                                                                \
	MethodInfo m_class::_gdvirtual_##m_name##_get_method_info() {                                                                                                                                               \
		MethodInfo method_info;                                                                                                                                                                                 \
		method_info.name = #m_name;                                                                                                                                                                             \
		method_info.flags = METHOD_FLAG_VIRTUAL;                                                                                                                                                                \
		method_info.return_val = GetTypeInfo<m_ret>::get_class_info();                                                                                                                                          \
		method_info.arguments.push_back(GetTypeInfo<m_type1>::get_class_info());                                                                                                                                \
                                                                                                                                                                                                                \
		return method_info;                                                                                                                                                                                     \
	}

#endif

bool TerrainLibrary::get_initialized() const {
	return _initialized;
}
void TerrainLibrary::set_initialized(const bool value) {
	_initialized = value;
}

bool TerrainLibrary::supports_caching() {
	RETURN_CALLD(bool, false, _supports_caching);
}
bool TerrainLibrary::_supports_caching() {
	return false;
}

//Materials
Ref<Material> TerrainLibrary::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>());

	return _materials[index];
}

Ref<Material> TerrainLibrary::material_lod_get(const int index) {
	ERR_FAIL_COND_V_MSG(_materials.size() == 0, Ref<Material>(), "Error! You should to add at least one material to TerrainLibrary! (By default it will use up to 5. See VoxelWorldDefault::_create_chunk().)");

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void TerrainLibrary::material_cache_get_key(Ref<TerrainChunk> chunk) {
	CALL(_material_cache_get_key, chunk);
}

void TerrainLibrary::_material_cache_get_key(Ref<TerrainChunk> chunk) {
}

Ref<TerrainMaterialCache> TerrainLibrary::material_cache_get(const int key) {
	RETURN_CALLP(Ref<TerrainMaterialCache>, _material_cache_get, key);
}

Ref<TerrainMaterialCache> TerrainLibrary::_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerrainMaterialCache>(), "This TerrainLibrary doesn't support cached materials!");
}

void TerrainLibrary::material_cache_unref(const int key) {
	CALL(_material_cache_unref, key);
}
void TerrainLibrary::_material_cache_unref(const int key) {
}

void TerrainLibrary::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void TerrainLibrary::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void TerrainLibrary::material_remove(const int index) {
	_materials.VREMOVE(index);
}

int TerrainLibrary::material_get_num() const {
	return _materials.size();
}

void TerrainLibrary::materials_clear() {
	_materials.clear();
}

Vector<Variant> TerrainLibrary::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void TerrainLibrary::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Liquid Materials
Ref<Material> TerrainLibrary::liquid_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _liquid_materials.size(), Ref<Material>());

	return _liquid_materials[index];
}

Ref<Material> TerrainLibrary::liquid_material_lod_get(const int index) {
	ERR_FAIL_COND_V_MSG(_liquid_materials.size() == 0, Ref<Material>(), "Error! You should to add at least one liquid to TerrainLibrary! (By default it will use up to 5. See VoxelWorldDefault::_create_chunk().)");

	if (index < 0) {
		return _liquid_materials[0];
	}

	if (index >= _liquid_materials.size()) {
		return _liquid_materials[_liquid_materials.size() - 1];
	}

	return _liquid_materials[index];
}

void TerrainLibrary::liquid_material_cache_get_key(Ref<TerrainChunk> chunk) {
	CALL(_liquid_material_cache_get_key, chunk);
}

void TerrainLibrary::_liquid_material_cache_get_key(Ref<TerrainChunk> chunk) {
}

Ref<TerrainMaterialCache> TerrainLibrary::liquid_material_cache_get(const int key) {
	RETURN_CALLP(Ref<TerrainMaterialCache>, _liquid_material_cache_get, key);
}

Ref<TerrainMaterialCache> TerrainLibrary::_liquid_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerrainMaterialCache>(), "This TerrainLibrary doesn't support cached liquid materials!");
}

void TerrainLibrary::liquid_material_cache_unref(const int key) {
	CALL(_liquid_material_cache_unref, key);
}
void TerrainLibrary::_liquid_material_cache_unref(const int key) {
}

void TerrainLibrary::liquid_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_liquid_materials.push_back(value);
}

void TerrainLibrary::liquid_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _liquid_materials.size());

	_liquid_materials.set(index, value);
}

void TerrainLibrary::liquid_material_remove(const int index) {
	_liquid_materials.VREMOVE(index);
}

int TerrainLibrary::liquid_material_get_num() const {
	return _liquid_materials.size();
}

void TerrainLibrary::liquid_materials_clear() {
	_liquid_materials.clear();
}

Vector<Variant> TerrainLibrary::liquid_materials_get() {
	VARIANT_ARRAY_GET(_liquid_materials);
}

void TerrainLibrary::liquid_materials_set(const Vector<Variant> &materials) {
	_liquid_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_liquid_materials.push_back(material);
	}
}

//Prop Materials
Ref<Material> TerrainLibrary::prop_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _prop_materials.size(), Ref<Material>());

	return _prop_materials[index];
}

Ref<Material> TerrainLibrary::prop_material_lod_get(const int index) {
	ERR_FAIL_COND_V_MSG(_prop_materials.size() == 0, Ref<Material>(), "Error! You should to add at least one prop material to TerrainLibrary! (By default it will use up to 5. See VoxelWorldDefault::_create_chunk().)");

	if (index < 0) {
		return _prop_materials[0];
	}

	if (index >= _prop_materials.size()) {
		return _prop_materials[_prop_materials.size() - 1];
	}

	return _prop_materials[index];
}

void TerrainLibrary::prop_material_cache_get_key(Ref<TerrainChunk> chunk) {
	CALL(_prop_material_cache_get_key, chunk);
}

void TerrainLibrary::_prop_material_cache_get_key(Ref<TerrainChunk> chunk) {
}

Ref<TerrainMaterialCache> TerrainLibrary::prop_material_cache_get(const int key) {
	RETURN_CALLP(Ref<TerrainMaterialCache>, _prop_material_cache_get, key);
}

Ref<TerrainMaterialCache> TerrainLibrary::_prop_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerrainMaterialCache>(), "This TerrainLibrary doesn't support cached prop materials!");
}

void TerrainLibrary::prop_material_cache_unref(const int key) {
	CALL(_prop_material_cache_unref, key);
}
void TerrainLibrary::_prop_material_cache_unref(const int key) {
}

void TerrainLibrary::prop_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_prop_materials.push_back(value);
}

void TerrainLibrary::prop_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _prop_materials.size());

	_prop_materials.set(index, value);
}

void TerrainLibrary::prop_material_remove(const int index) {
	_prop_materials.VREMOVE(index);
}

int TerrainLibrary::prop_material_get_num() const {
	return _prop_materials.size();
}

void TerrainLibrary::prop_materials_clear() {
	_prop_materials.clear();
}

Vector<Variant> TerrainLibrary::prop_materials_get() {
	VARIANT_ARRAY_GET(_prop_materials);
}

void TerrainLibrary::prop_materials_set(const Vector<Variant> &materials) {
	VARIANT_ARRAY_SET(materials, _prop_materials, Material);
}

//Surfaces
Ref<TerrainSurface> TerrainLibrary::voxel_surface_get(const int index) {
	return Ref<TerrainSurface>();
}
void TerrainLibrary::voxel_surface_add(Ref<TerrainSurface> value) {
}
void TerrainLibrary::voxel_surface_set(int index, Ref<TerrainSurface> value) {
}
void TerrainLibrary::voxel_surface_remove(const int index) {
}
int TerrainLibrary::voxel_surface_get_num() const {
	return 0;
}
void TerrainLibrary::voxel_surfaces_clear() {
}

Ref<PackedScene> TerrainLibrary::scene_get(const int id) {
	return Ref<PackedScene>();
}
void TerrainLibrary::scene_add(Ref<PackedScene> value) {
}
void TerrainLibrary::scene_set(int id, Ref<PackedScene> value) {
}
void TerrainLibrary::scene_remove(const int id) {
}
int TerrainLibrary::scene_get_num() const {
	return 0;
}
void TerrainLibrary::scenes_clear() {
}

#ifdef PROPS_PRESENT
Ref<PropData> TerrainLibrary::prop_get(const int id) {
	return Ref<PropData>();
}
void TerrainLibrary::prop_add(Ref<PropData> value) {
}
bool TerrainLibrary::prop_has(const Ref<PropData> &value) const {
	return false;
}
void TerrainLibrary::prop_set(int id, Ref<PropData> value) {
}
void TerrainLibrary::prop_remove(const int id) {
}
int TerrainLibrary::prop_get_num() const {
	return 0;
}
void TerrainLibrary::props_clear() {
}

Rect2 TerrainLibrary::get_prop_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}
#endif

//Rects
void TerrainLibrary::refresh_rects() {
	_initialized = true;
}

void TerrainLibrary::setup_material_albedo(int material_index, Ref<Texture> texture) {
	if (has_method("_setup_material_albedo")) {
		CALL(_setup_material_albedo, material_index, texture);
	}
}

TerrainLibrary::TerrainLibrary() {
	_initialized = false;
}

TerrainLibrary::~TerrainLibrary() {
	_materials.clear();
	_liquid_materials.clear();
	_prop_materials.clear();
}

#if VERSION_MAJOR >= 4
TMCGDVIRTUAL1(TerrainLibrary, _material_cache_get_key, Ref<TerrainChunk>);
TMCGDVIRTUAL1R(TerrainLibrary, Ref<TerrainMaterialCache>, _material_cache_get, int);
TMCGDVIRTUAL1(TerrainLibrary, _material_cache_unref, int);

TMCGDVIRTUAL1(TerrainLibrary, _liquid_material_cache_get_key, Ref<TerrainChunk>);
TMCGDVIRTUAL1R(TerrainLibrary, Ref<TerrainMaterialCache>, _liquid_material_cache_get, int);
TMCGDVIRTUAL1(TerrainLibrary, _liquid_material_cache_unref, int);

TMCGDVIRTUAL1(TerrainLibrary, _prop_material_cache_get_key, Ref<TerrainChunk>);
TMCGDVIRTUAL1R(TerrainLibrary, Ref<TerrainMaterialCache>, _prop_material_cache_get, int);
TMCGDVIRTUAL1(TerrainLibrary, _prop_material_cache_unref, int);
#endif

void TerrainLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &TerrainLibrary::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &TerrainLibrary::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized", PROPERTY_HINT_NONE, "", 0), "set_initialized", "get_initialized");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_supports_caching"));
#else
	GDVIRTUAL_BIND(_supports_caching);
#endif

	ClassDB::bind_method(D_METHOD("_supports_caching"), &TerrainLibrary::_supports_caching);
	ClassDB::bind_method(D_METHOD("supports_caching"), &TerrainLibrary::supports_caching);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::INT, "material_index"), PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));
#else
	GDVIRTUAL_BIND(_setup_material_albedo, "material_index", "texture");
#endif

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerrainMaterialCache"), "_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_material_cache_get, "key");
	GDVIRTUAL_BIND(_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("material_get", "index"), &TerrainLibrary::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &TerrainLibrary::material_lod_get);

	ClassDB::bind_method(D_METHOD("material_cache_get_key", "chunk"), &TerrainLibrary::material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_material_cache_get_key", "chunk"), &TerrainLibrary::_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("material_cache_get", "key"), &TerrainLibrary::material_cache_get);
	ClassDB::bind_method(D_METHOD("_material_cache_get", "key"), &TerrainLibrary::_material_cache_get);
	ClassDB::bind_method(D_METHOD("material_cache_unref", "key"), &TerrainLibrary::material_cache_unref);
	ClassDB::bind_method(D_METHOD("_material_cache_unref", "key"), &TerrainLibrary::_material_cache_unref);

	ClassDB::bind_method(D_METHOD("material_add", "value"), &TerrainLibrary::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &TerrainLibrary::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &TerrainLibrary::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &TerrainLibrary::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &TerrainLibrary::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &TerrainLibrary::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &TerrainLibrary::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_liquid_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerrainMaterialCache"), "_liquid_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_liquid_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_liquid_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_liquid_material_cache_get, "key");
	GDVIRTUAL_BIND(_liquid_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("liquid_material_get", "index"), &TerrainLibrary::liquid_material_get);
	ClassDB::bind_method(D_METHOD("liquid_material_lod_get", "index"), &TerrainLibrary::liquid_material_lod_get);

	ClassDB::bind_method(D_METHOD("liquid_material_cache_get_key", "chunk"), &TerrainLibrary::liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get_key", "chunk"), &TerrainLibrary::_liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_get", "key"), &TerrainLibrary::liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get", "key"), &TerrainLibrary::_liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_unref", "key"), &TerrainLibrary::liquid_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_unref", "key"), &TerrainLibrary::_liquid_material_cache_unref);

	ClassDB::bind_method(D_METHOD("liquid_material_add", "value"), &TerrainLibrary::liquid_material_add);
	ClassDB::bind_method(D_METHOD("liquid_material_set", "index", "value"), &TerrainLibrary::liquid_material_set);
	ClassDB::bind_method(D_METHOD("liquid_material_remove", "index"), &TerrainLibrary::liquid_material_remove);
	ClassDB::bind_method(D_METHOD("liquid_material_get_num"), &TerrainLibrary::liquid_material_get_num);
	ClassDB::bind_method(D_METHOD("liquid_materials_clear"), &TerrainLibrary::liquid_materials_clear);

	ClassDB::bind_method(D_METHOD("liquid_materials_get"), &TerrainLibrary::liquid_materials_get);
	ClassDB::bind_method(D_METHOD("liquid_materials_set"), &TerrainLibrary::liquid_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "liquid_materials_set", "liquid_materials_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_prop_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerrainMaterialCache"), "_prop_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_prop_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_prop_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_prop_material_cache_get, "key");
	GDVIRTUAL_BIND(_prop_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("prop_material_get", "index"), &TerrainLibrary::prop_material_get);
	ClassDB::bind_method(D_METHOD("prop_material_lod_get", "index"), &TerrainLibrary::prop_material_lod_get);

	ClassDB::bind_method(D_METHOD("prop_material_cache_get_key", "chunk"), &TerrainLibrary::prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get_key", "chunk"), &TerrainLibrary::_prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("prop_material_cache_get", "key"), &TerrainLibrary::prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get", "key"), &TerrainLibrary::_prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_unref", "key"), &TerrainLibrary::prop_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_unref", "key"), &TerrainLibrary::_prop_material_cache_unref);

	ClassDB::bind_method(D_METHOD("prop_material_add", "value"), &TerrainLibrary::prop_material_add);
	ClassDB::bind_method(D_METHOD("prop_material_set", "index", "value"), &TerrainLibrary::prop_material_set);
	ClassDB::bind_method(D_METHOD("prop_material_remove", "index"), &TerrainLibrary::prop_material_remove);
	ClassDB::bind_method(D_METHOD("prop_material_get_num"), &TerrainLibrary::prop_material_get_num);
	ClassDB::bind_method(D_METHOD("prop_materials_clear"), &TerrainLibrary::prop_materials_clear);

	ClassDB::bind_method(D_METHOD("prop_materials_get"), &TerrainLibrary::prop_materials_get);
	ClassDB::bind_method(D_METHOD("prop_materials_set"), &TerrainLibrary::prop_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "prop_materials_set", "prop_materials_get");

	ClassDB::bind_method(D_METHOD("voxel_surface_get", "index"), &TerrainLibrary::voxel_surface_get);
	ClassDB::bind_method(D_METHOD("voxel_surface_add", "value"), &TerrainLibrary::voxel_surface_add);
	ClassDB::bind_method(D_METHOD("voxel_surface_set", "index", "surface"), &TerrainLibrary::voxel_surface_set);
	ClassDB::bind_method(D_METHOD("voxel_surface_remove", "index"), &TerrainLibrary::voxel_surface_remove);
	ClassDB::bind_method(D_METHOD("voxel_surface_get_num"), &TerrainLibrary::voxel_surface_get_num);
	ClassDB::bind_method(D_METHOD("voxel_surfaces_clear"), &TerrainLibrary::voxel_surfaces_clear);

	ClassDB::bind_method(D_METHOD("scene_get", "index"), &TerrainLibrary::scene_get);
	ClassDB::bind_method(D_METHOD("scene_add", "value"), &TerrainLibrary::scene_add);
	ClassDB::bind_method(D_METHOD("scene_set", "index", "value"), &TerrainLibrary::scene_set);
	ClassDB::bind_method(D_METHOD("scene_remove", "index"), &TerrainLibrary::scene_remove);
	ClassDB::bind_method(D_METHOD("scene_get_num"), &TerrainLibrary::scene_get_num);
	ClassDB::bind_method(D_METHOD("scenes_clear"), &TerrainLibrary::scenes_clear);

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_get", "id"), &TerrainLibrary::prop_get);
	ClassDB::bind_method(D_METHOD("prop_add", "value"), &TerrainLibrary::prop_add);
	ClassDB::bind_method(D_METHOD("prop_has", "prop"), &TerrainLibrary::prop_has);
	ClassDB::bind_method(D_METHOD("prop_set", "id", "surface"), &TerrainLibrary::prop_set);
	ClassDB::bind_method(D_METHOD("prop_remove", "id"), &TerrainLibrary::prop_remove);
	ClassDB::bind_method(D_METHOD("prop_get_num"), &TerrainLibrary::prop_get_num);
	ClassDB::bind_method(D_METHOD("props_clear"), &TerrainLibrary::props_clear);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &TerrainLibrary::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "material_index", "texture"), &TerrainLibrary::setup_material_albedo);

	BIND_CONSTANT(MATERIAL_INDEX_TERRAIN);
	BIND_CONSTANT(MATERIAL_INDEX_LIQUID);
	BIND_CONSTANT(MATERIAL_INDEX_PROP);
}
