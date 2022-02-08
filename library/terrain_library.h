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

#ifndef TERRAIN_LIBRARY_H
#define TERRAIN_LIBRARY_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"

// These are needed here to use incomplete classes as vmethod arguments (godot4)

#define TMHGDVIRTUAL1(m_name, m_type1)                                       \
	StringName _gdvirtual_##m_name##_sn = #m_name;                           \
	mutable bool _gdvirtual_##m_name##_initialized = false;                  \
	mutable GDNativeExtensionClassCallVirtual _gdvirtual_##m_name = nullptr; \
	bool _gdvirtual_##m_name##_call(m_type1 arg1);                           \
	bool _gdvirtual_##m_name##_overridden() const;                           \
	static MethodInfo _gdvirtual_##m_name##_get_method_info();

#define TMHGDVIRTUAL1R(m_ret, m_name, m_type1)                               \
	StringName _gdvirtual_##m_name##_sn = #m_name;                           \
	mutable bool _gdvirtual_##m_name##_initialized = false;                  \
	mutable GDNativeExtensionClassCallVirtual _gdvirtual_##m_name = nullptr; \
	bool _gdvirtual_##m_name##_call(m_type1 arg1, m_ret &r_ret);             \
	bool _gdvirtual_##m_name##_overridden() const;                           \
	static MethodInfo _gdvirtual_##m_name##_get_method_info();

#else
#include "core/resource.h"
#endif

#include "core/math/rect2.h"
#include "scene/resources/material.h"

#include "../data/terrain_light.h"
#include "terrain_surface.h"

#include "../defines.h"

#if GODOT4
#define Texture Texture2D
#endif

class TerrainMaterialCache;
class TerrainSurface;
class TerrainMesher;
class PackedScene;
class TerrainChunk;
#ifdef PROPS_PRESENT
class PropData;
#endif

class TerrainLibrary : public Resource {
	GDCLASS(TerrainLibrary, Resource)

public:
	enum {
		MATERIAL_INDEX_TERRAIN = 0,
		MATERIAL_INDEX_LIQUID = 1,
		MATERIAL_INDEX_PROP = 2,
	};

public:
	bool get_initialized() const;
	void set_initialized(const bool value);

	bool supports_caching();
	virtual bool _supports_caching();

	Ref<Material> material_get(const int index);
	Ref<Material> material_lod_get(const int index);

	void material_cache_get_key(Ref<TerrainChunk> chunk);
	virtual void _material_cache_get_key(Ref<TerrainChunk> chunk);
	Ref<TerrainMaterialCache> material_cache_get(const int key);
	virtual Ref<TerrainMaterialCache> _material_cache_get(const int key);
	void material_cache_unref(const int key);
	virtual void _material_cache_unref(const int key);

	void material_add(const Ref<Material> &value);
	void material_set(const int index, const Ref<Material> &value);
	void material_remove(const int index);
	int material_get_num() const;
	void materials_clear();

	Vector<Variant> materials_get();
	void materials_set(const Vector<Variant> &materials);

	Ref<Material> liquid_material_get(const int index);
	Ref<Material> liquid_material_lod_get(const int index);

	void liquid_material_cache_get_key(Ref<TerrainChunk> chunk);
	virtual void _liquid_material_cache_get_key(Ref<TerrainChunk> chunk);
	Ref<TerrainMaterialCache> liquid_material_cache_get(const int key);
	virtual Ref<TerrainMaterialCache> _liquid_material_cache_get(const int key);
	void liquid_material_cache_unref(const int key);
	virtual void _liquid_material_cache_unref(const int key);

	void liquid_material_add(const Ref<Material> &value);
	void liquid_material_set(const int index, const Ref<Material> &value);
	void liquid_material_remove(const int index);
	int liquid_material_get_num() const;
	void liquid_materials_clear();

	Vector<Variant> liquid_materials_get();
	void liquid_materials_set(const Vector<Variant> &materials);

	Ref<Material> prop_material_get(const int index);
	Ref<Material> prop_material_lod_get(const int index);

	void prop_material_cache_get_key(Ref<TerrainChunk> chunk);
	virtual void _prop_material_cache_get_key(Ref<TerrainChunk> chunk);
	Ref<TerrainMaterialCache> prop_material_cache_get(const int key);
	virtual Ref<TerrainMaterialCache> _prop_material_cache_get(const int key);
	void prop_material_cache_unref(const int key);
	virtual void _prop_material_cache_unref(const int key);

	void prop_material_add(const Ref<Material> &value);
	void prop_material_set(const int index, const Ref<Material> &value);
	void prop_material_remove(const int index);
	int prop_material_get_num() const;
	void prop_materials_clear();

	Vector<Variant> prop_materials_get();
	void prop_materials_set(const Vector<Variant> &materials);

	virtual Ref<TerrainSurface> voxel_surface_get(const int index);
	virtual void voxel_surface_add(Ref<TerrainSurface> value);
	virtual void voxel_surface_set(const int index, Ref<TerrainSurface> value);
	virtual void voxel_surface_remove(const int index);
	virtual int voxel_surface_get_num() const;
	virtual void voxel_surfaces_clear();

	virtual Ref<PackedScene> scene_get(const int id);
	virtual void scene_add(Ref<PackedScene> value);
	virtual void scene_set(const int id, Ref<PackedScene> value);
	virtual void scene_remove(const int id);
	virtual int scene_get_num() const;
	virtual void scenes_clear();

#ifdef PROPS_PRESENT
	virtual Ref<PropData> prop_get(const int id);
	virtual void prop_add(Ref<PropData> value);
	virtual bool prop_has(const Ref<PropData> &value) const;
	virtual void prop_set(const int id, Ref<PropData> value);
	virtual void prop_remove(const int id);
	virtual int prop_get_num() const;
	virtual void props_clear();

	virtual Rect2 get_prop_uv_rect(const Ref<Texture> &texture);
#endif

	virtual void refresh_rects();

	void setup_material_albedo(int material_index, Ref<Texture> texture);

#if VERSION_MAJOR >= 4
	GDVIRTUAL0R(bool, _supports_caching);

	GDVIRTUAL2(_setup_material_albedo, int, Ref<Texture>);

	TMHGDVIRTUAL1(_material_cache_get_key, Ref<TerrainChunk>);
	TMHGDVIRTUAL1R(Ref<TerrainMaterialCache>, _material_cache_get, int);
	TMHGDVIRTUAL1(_material_cache_unref, int);

	TMHGDVIRTUAL1(_liquid_material_cache_get_key, Ref<TerrainChunk>);
	TMHGDVIRTUAL1R(Ref<TerrainMaterialCache>, _liquid_material_cache_get, int);
	TMHGDVIRTUAL1(_liquid_material_cache_unref, int);

	TMHGDVIRTUAL1(_prop_material_cache_get_key, Ref<TerrainChunk>);
	TMHGDVIRTUAL1R(Ref<TerrainMaterialCache>, _prop_material_cache_get, int);
	TMHGDVIRTUAL1(_prop_material_cache_unref, int);
#endif

	TerrainLibrary();
	~TerrainLibrary();

protected:
	static void _bind_methods();

	bool _initialized;
	Vector<Ref<Material>> _materials;
	Vector<Ref<Material>> _liquid_materials;
	Vector<Ref<Material>> _prop_materials;
};

#endif // TERRAIN_LIBRARY_H
