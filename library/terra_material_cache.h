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

#ifndef TERRA_MATERIAL_CACHE_H
#define TERRA_MATERIAL_CACHE_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#include "core/math/color.h"
#include "core/templates/vector.h"
#else
#include "core/color.h"
#include "core/resource.h"
#include "core/vector.h"
#endif

#include "core/math/rect2.h"
#include "scene/resources/material.h"

#include "terraman_library.h"

#include "../defines.h"

class TerramanLibrary;

class TerraMaterialCache : public Resource {
	GDCLASS(TerraMaterialCache, Resource)

public:
	Ref<Material> material_get(const int index);
	Ref<Material> material_lod_get(const int index);
	void material_add(const Ref<Material> &value);
	void material_set(const int index, const Ref<Material> &value);
	void material_remove(const int index);
	int material_get_num() const;
	void materials_clear();

	Vector<Variant> materials_get();
	void materials_set(const Vector<Variant> &materials);

	virtual Ref<TerraSurface> voxel_surface_get(const int index);
	virtual Ref<TerraSurface> voxel_surface_id_get(const int id);
	virtual void voxel_surface_add(Ref<TerraSurface> value);
	virtual void voxel_surface_set(const int index, Ref<TerraSurface> value);
	virtual void voxel_surface_remove(const int index);
	virtual int voxel_surface_get_num() const;
	virtual void voxel_surfaces_clear();

	virtual void refresh_rects();

	void setup_material_albedo(Ref<Texture> texture);

	TerraMaterialCache();
	~TerraMaterialCache();

protected:
	static void _bind_methods();

	Vector<Ref<TerraSurface>> _surfaces;
	Vector<Ref<Material>> _materials;
	//Ref<TextureMerger> merger; inherited
	int material_users;
};

#endif
