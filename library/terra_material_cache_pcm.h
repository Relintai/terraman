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

#ifndef TERRA_MATERIAL_CACHE_PCM_H
#define TERRA_MATERIAL_CACHE_PCM_H

#include "terra_material_cache.h"

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

#include "../defines.h"

class TerraSurface;

class TerraMaterialCachePCM : public TerraMaterialCache {
	GDCLASS(TerraMaterialCachePCM, TerraMaterialCache);

public:
	Ref<TerraSurface> voxel_surface_get(const int index);
	void voxel_surface_add(Ref<TerraSurface> value);
	void voxel_surface_set(const int index, Ref<TerraSurface> value);
	void voxel_surface_remove(const int index);
	int voxel_surface_get_num() const;
	void voxel_surfaces_clear();

	void refresh_rects();

	TerraMaterialCachePCM();
	~TerraMaterialCachePCM();

protected:
	static void _bind_methods();

	//Ref<TextureMerger> merger;
};

#endif
