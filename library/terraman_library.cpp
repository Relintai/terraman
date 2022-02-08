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

#include "terraman_library.h"

#include "../world/terra_chunk.h"

#include "terra_material_cache.h"

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

bool TerramanLibrary::get_initialized() const {
	return _initialized;
}
void TerramanLibrary::set_initialized(const bool value) {
	_initialized = value;
}

bool TerramanLibrary::supports_caching() {
	RETURN_CALLD(bool, false, _supports_caching);
}
bool TerramanLibrary::_supports_caching() {
	return false;
}

//Materials
Ref<Material> TerramanLibrary::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

Ref<Material> TerramanLibrary::material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void TerramanLibrary::material_cache_get_key(Ref<TerraChunk> chunk) {
	CALL(_material_cache_get_key, chunk);
}

void TerramanLibrary::_material_cache_get_key(Ref<TerraChunk> chunk) {
}

Ref<TerraMaterialCache> TerramanLibrary::material_cache_get(const int key) {
	RETURN_CALLP(Ref<TerraMaterialCache>, _material_cache_get, key);
}

Ref<TerraMaterialCache> TerramanLibrary::_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerraMaterialCache>(), "This TerramanLibrary doesn't support cached materials!");
}

void TerramanLibrary::material_cache_unref(const int key) {
	CALL(_material_cache_unref, key);
}
void TerramanLibrary::_material_cache_unref(const int key) {
}

void TerramanLibrary::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void TerramanLibrary::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void TerramanLibrary::material_remove(const int index) {
	_materials.VREMOVE(index);
}

int TerramanLibrary::material_get_num() const {
	return _materials.size();
}

void TerramanLibrary::materials_clear() {
	_materials.clear();
}

Vector<Variant> TerramanLibrary::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void TerramanLibrary::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Liquid Materials
Ref<Material> TerramanLibrary::liquid_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _liquid_materials.size(), Ref<Material>(NULL));

	return _liquid_materials[index];
}

Ref<Material> TerramanLibrary::liquid_material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _liquid_materials[0];
	}

	if (index >= _liquid_materials.size()) {
		return _liquid_materials[_liquid_materials.size() - 1];
	}

	return _liquid_materials[index];
}

void TerramanLibrary::liquid_material_cache_get_key(Ref<TerraChunk> chunk) {
	CALL(_liquid_material_cache_get_key, chunk);
}

void TerramanLibrary::_liquid_material_cache_get_key(Ref<TerraChunk> chunk) {
}

Ref<TerraMaterialCache> TerramanLibrary::liquid_material_cache_get(const int key) {
	RETURN_CALLP(Ref<TerraMaterialCache>, _liquid_material_cache_get, key);
}

Ref<TerraMaterialCache> TerramanLibrary::_liquid_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerraMaterialCache>(), "This TerramanLibrary doesn't support cached liquid materials!");
}

void TerramanLibrary::liquid_material_cache_unref(const int key) {
	CALL(_liquid_material_cache_unref, key);
}
void TerramanLibrary::_liquid_material_cache_unref(const int key) {
}

void TerramanLibrary::liquid_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_liquid_materials.push_back(value);
}

void TerramanLibrary::liquid_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _liquid_materials.size());

	_liquid_materials.set(index, value);
}

void TerramanLibrary::liquid_material_remove(const int index) {
	_liquid_materials.VREMOVE(index);
}

int TerramanLibrary::liquid_material_get_num() const {
	return _liquid_materials.size();
}

void TerramanLibrary::liquid_materials_clear() {
	_liquid_materials.clear();
}

Vector<Variant> TerramanLibrary::liquid_materials_get() {
	VARIANT_ARRAY_GET(_liquid_materials);
}

void TerramanLibrary::liquid_materials_set(const Vector<Variant> &materials) {
	_liquid_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_liquid_materials.push_back(material);
	}
}

//Prop Materials
Ref<Material> TerramanLibrary::prop_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _prop_materials.size(), Ref<Material>(NULL));

	return _prop_materials[index];
}

Ref<Material> TerramanLibrary::prop_material_lod_get(const int index) {
	ERR_FAIL_COND_V(_prop_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _prop_materials[0];
	}

	if (index >= _prop_materials.size()) {
		return _prop_materials[_prop_materials.size() - 1];
	}

	return _prop_materials[index];
}

void TerramanLibrary::prop_material_cache_get_key(Ref<TerraChunk> chunk) {
	CALL(_prop_material_cache_get_key, chunk);
}

void TerramanLibrary::_prop_material_cache_get_key(Ref<TerraChunk> chunk) {
}

Ref<TerraMaterialCache> TerramanLibrary::prop_material_cache_get(const int key) {
	RETURN_CALLP(Ref<TerraMaterialCache>, _prop_material_cache_get, key);
}

Ref<TerraMaterialCache> TerramanLibrary::_prop_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerraMaterialCache>(), "This TerramanLibrary doesn't support cached prop materials!");
}

void TerramanLibrary::prop_material_cache_unref(const int key) {
	CALL(_prop_material_cache_unref, key);
}
void TerramanLibrary::_prop_material_cache_unref(const int key) {
}

void TerramanLibrary::prop_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_prop_materials.push_back(value);
}

void TerramanLibrary::prop_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _prop_materials.size());

	_prop_materials.set(index, value);
}

void TerramanLibrary::prop_material_remove(const int index) {
	_prop_materials.VREMOVE(index);
}

int TerramanLibrary::prop_material_get_num() const {
	return _prop_materials.size();
}

void TerramanLibrary::prop_materials_clear() {
	_prop_materials.clear();
}

Vector<Variant> TerramanLibrary::prop_materials_get() {
	VARIANT_ARRAY_GET(_prop_materials);
}

void TerramanLibrary::prop_materials_set(const Vector<Variant> &materials) {
	VARIANT_ARRAY_SET(materials, _prop_materials, Material);
}

//Surfaces
Ref<TerraSurface> TerramanLibrary::voxel_surface_get(const int index) {
	return Ref<TerraSurface>();
}
void TerramanLibrary::voxel_surface_add(Ref<TerraSurface> value) {
}
void TerramanLibrary::voxel_surface_set(int index, Ref<TerraSurface> value) {
}
void TerramanLibrary::voxel_surface_remove(const int index) {
}
int TerramanLibrary::voxel_surface_get_num() const {
	return 0;
}
void TerramanLibrary::voxel_surfaces_clear() {
}

Ref<PackedScene> TerramanLibrary::scene_get(const int id) {
	return Ref<PackedScene>();
}
void TerramanLibrary::scene_add(Ref<PackedScene> value) {
}
void TerramanLibrary::scene_set(int id, Ref<PackedScene> value) {
}
void TerramanLibrary::scene_remove(const int id) {
}
int TerramanLibrary::scene_get_num() const {
	return 0;
}
void TerramanLibrary::scenes_clear() {
}

#ifdef PROPS_PRESENT
Ref<PropData> TerramanLibrary::prop_get(const int id) {
	return Ref<PropData>();
}
void TerramanLibrary::prop_add(Ref<PropData> value) {
}
bool TerramanLibrary::prop_has(const Ref<PropData> &value) const {
	return false;
}
void TerramanLibrary::prop_set(int id, Ref<PropData> value) {
}
void TerramanLibrary::prop_remove(const int id) {
}
int TerramanLibrary::prop_get_num() const {
	return 0;
}
void TerramanLibrary::props_clear() {
}

Rect2 TerramanLibrary::get_prop_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}
#endif

//Rects
void TerramanLibrary::refresh_rects() {
	_initialized = true;
}

void TerramanLibrary::setup_material_albedo(int material_index, Ref<Texture> texture) {
	if (has_method("_setup_material_albedo")) {
		CALL(_setup_material_albedo, material_index, texture);
	}
}

TerramanLibrary::TerramanLibrary() {
	_initialized = false;
}

TerramanLibrary::~TerramanLibrary() {
	_materials.clear();
	_liquid_materials.clear();
	_prop_materials.clear();
}

#if VERSION_MAJOR >= 4
TMCGDVIRTUAL1(TerramanLibrary, _material_cache_get_key, Ref<TerraChunk>);
TMCGDVIRTUAL1R(TerramanLibrary, Ref<TerraMaterialCache>, _material_cache_get, int);
TMCGDVIRTUAL1(TerramanLibrary, _material_cache_unref, int);

TMCGDVIRTUAL1(TerramanLibrary, _liquid_material_cache_get_key, Ref<TerraChunk>);
TMCGDVIRTUAL1R(TerramanLibrary, Ref<TerraMaterialCache>, _liquid_material_cache_get, int);
TMCGDVIRTUAL1(TerramanLibrary, _liquid_material_cache_unref, int);

TMCGDVIRTUAL1(TerramanLibrary, _prop_material_cache_get_key, Ref<TerraChunk>);
TMCGDVIRTUAL1R(TerramanLibrary, Ref<TerraMaterialCache>, _prop_material_cache_get, int);
TMCGDVIRTUAL1(TerramanLibrary, _prop_material_cache_unref, int);
#endif

void TerramanLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &TerramanLibrary::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &TerramanLibrary::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized", PROPERTY_HINT_NONE, "", 0), "set_initialized", "get_initialized");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_supports_caching"));
#else
	GDVIRTUAL_BIND(_supports_caching);
#endif

	ClassDB::bind_method(D_METHOD("_supports_caching"), &TerramanLibrary::_supports_caching);
	ClassDB::bind_method(D_METHOD("supports_caching"), &TerramanLibrary::supports_caching);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::INT, "material_index"), PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));
#else
	GDVIRTUAL_BIND(_setup_material_albedo, "material_index", "texture");
#endif

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerraMaterialCache"), "_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_material_cache_get, "key");
	GDVIRTUAL_BIND(_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("material_get", "index"), &TerramanLibrary::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &TerramanLibrary::material_lod_get);

	ClassDB::bind_method(D_METHOD("material_cache_get_key", "chunk"), &TerramanLibrary::material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_material_cache_get_key", "chunk"), &TerramanLibrary::_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("material_cache_get", "key"), &TerramanLibrary::material_cache_get);
	ClassDB::bind_method(D_METHOD("_material_cache_get", "key"), &TerramanLibrary::_material_cache_get);
	ClassDB::bind_method(D_METHOD("material_cache_unref", "key"), &TerramanLibrary::material_cache_unref);
	ClassDB::bind_method(D_METHOD("_material_cache_unref", "key"), &TerramanLibrary::_material_cache_unref);

	ClassDB::bind_method(D_METHOD("material_add", "value"), &TerramanLibrary::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &TerramanLibrary::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &TerramanLibrary::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &TerramanLibrary::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &TerramanLibrary::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &TerramanLibrary::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &TerramanLibrary::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_liquid_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerraMaterialCache"), "_liquid_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_liquid_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_liquid_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_liquid_material_cache_get, "key");
	GDVIRTUAL_BIND(_liquid_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("liquid_material_get", "index"), &TerramanLibrary::liquid_material_get);
	ClassDB::bind_method(D_METHOD("liquid_material_lod_get", "index"), &TerramanLibrary::liquid_material_lod_get);

	ClassDB::bind_method(D_METHOD("liquid_material_cache_get_key", "chunk"), &TerramanLibrary::liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get_key", "chunk"), &TerramanLibrary::_liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_get", "key"), &TerramanLibrary::liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get", "key"), &TerramanLibrary::_liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_unref", "key"), &TerramanLibrary::liquid_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_unref", "key"), &TerramanLibrary::_liquid_material_cache_unref);

	ClassDB::bind_method(D_METHOD("liquid_material_add", "value"), &TerramanLibrary::liquid_material_add);
	ClassDB::bind_method(D_METHOD("liquid_material_set", "index", "value"), &TerramanLibrary::liquid_material_set);
	ClassDB::bind_method(D_METHOD("liquid_material_remove", "index"), &TerramanLibrary::liquid_material_remove);
	ClassDB::bind_method(D_METHOD("liquid_material_get_num"), &TerramanLibrary::liquid_material_get_num);
	ClassDB::bind_method(D_METHOD("liquid_materials_clear"), &TerramanLibrary::liquid_materials_clear);

	ClassDB::bind_method(D_METHOD("liquid_materials_get"), &TerramanLibrary::liquid_materials_get);
	ClassDB::bind_method(D_METHOD("liquid_materials_set"), &TerramanLibrary::liquid_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "liquid_materials_set", "liquid_materials_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_prop_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerraMaterialCache"), "_prop_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_prop_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_prop_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_prop_material_cache_get, "key");
	GDVIRTUAL_BIND(_prop_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("prop_material_get", "index"), &TerramanLibrary::prop_material_get);
	ClassDB::bind_method(D_METHOD("prop_material_lod_get", "index"), &TerramanLibrary::prop_material_lod_get);

	ClassDB::bind_method(D_METHOD("prop_material_cache_get_key", "chunk"), &TerramanLibrary::prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get_key", "chunk"), &TerramanLibrary::_prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("prop_material_cache_get", "key"), &TerramanLibrary::prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get", "key"), &TerramanLibrary::_prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_unref", "key"), &TerramanLibrary::prop_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_unref", "key"), &TerramanLibrary::_prop_material_cache_unref);

	ClassDB::bind_method(D_METHOD("prop_material_add", "value"), &TerramanLibrary::prop_material_add);
	ClassDB::bind_method(D_METHOD("prop_material_set", "index", "value"), &TerramanLibrary::prop_material_set);
	ClassDB::bind_method(D_METHOD("prop_material_remove", "index"), &TerramanLibrary::prop_material_remove);
	ClassDB::bind_method(D_METHOD("prop_material_get_num"), &TerramanLibrary::prop_material_get_num);
	ClassDB::bind_method(D_METHOD("prop_materials_clear"), &TerramanLibrary::prop_materials_clear);

	ClassDB::bind_method(D_METHOD("prop_materials_get"), &TerramanLibrary::prop_materials_get);
	ClassDB::bind_method(D_METHOD("prop_materials_set"), &TerramanLibrary::prop_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "prop_materials_set", "prop_materials_get");

	ClassDB::bind_method(D_METHOD("voxel_surface_get", "index"), &TerramanLibrary::voxel_surface_get);
	ClassDB::bind_method(D_METHOD("voxel_surface_add", "value"), &TerramanLibrary::voxel_surface_add);
	ClassDB::bind_method(D_METHOD("voxel_surface_set", "index", "surface"), &TerramanLibrary::voxel_surface_set);
	ClassDB::bind_method(D_METHOD("voxel_surface_remove", "index"), &TerramanLibrary::voxel_surface_remove);
	ClassDB::bind_method(D_METHOD("voxel_surface_get_num"), &TerramanLibrary::voxel_surface_get_num);
	ClassDB::bind_method(D_METHOD("voxel_surfaces_clear"), &TerramanLibrary::voxel_surfaces_clear);

	ClassDB::bind_method(D_METHOD("scene_get", "index"), &TerramanLibrary::scene_get);
	ClassDB::bind_method(D_METHOD("scene_add", "value"), &TerramanLibrary::scene_add);
	ClassDB::bind_method(D_METHOD("scene_set", "index", "value"), &TerramanLibrary::scene_set);
	ClassDB::bind_method(D_METHOD("scene_remove", "index"), &TerramanLibrary::scene_remove);
	ClassDB::bind_method(D_METHOD("scene_get_num"), &TerramanLibrary::scene_get_num);
	ClassDB::bind_method(D_METHOD("scenes_clear"), &TerramanLibrary::scenes_clear);

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_get", "id"), &TerramanLibrary::prop_get);
	ClassDB::bind_method(D_METHOD("prop_add", "value"), &TerramanLibrary::prop_add);
	ClassDB::bind_method(D_METHOD("prop_has", "prop"), &TerramanLibrary::prop_has);
	ClassDB::bind_method(D_METHOD("prop_set", "id", "surface"), &TerramanLibrary::prop_set);
	ClassDB::bind_method(D_METHOD("prop_remove", "id"), &TerramanLibrary::prop_remove);
	ClassDB::bind_method(D_METHOD("prop_get_num"), &TerramanLibrary::prop_get_num);
	ClassDB::bind_method(D_METHOD("props_clear"), &TerramanLibrary::props_clear);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &TerramanLibrary::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "material_index", "texture"), &TerramanLibrary::setup_material_albedo);

	BIND_CONSTANT(MATERIAL_INDEX_TERRAS);
	BIND_CONSTANT(MATERIAL_INDEX_LIQUID);
	BIND_CONSTANT(MATERIAL_INDEX_PROP);
}
