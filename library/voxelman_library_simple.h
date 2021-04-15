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

#ifndef TERRAMAN_LIBRARY_SIMPLE_H
#define TERRAMAN_LIBRARY_SIMPLE_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#else
#include "core/resource.h"
#endif

#include "voxelman_library.h"

#include "scene/resources/material.h"

#include "../data/voxel_light.h"
#include "voxel_surface_simple.h"

class TerraSurfaceSimple;
class TerraMesher;

class TerramanLibrarySimple : public TerramanLibrary {
	GDCLASS(TerramanLibrarySimple, TerramanLibrary)

public:
	int get_atlas_columns() const;
	void set_atlas_columns(int s);

	int get_atlas_rows() const;
	void set_atlas_rows(int s);

	Ref<TerraSurface> voxel_surface_get(const int index);
	void voxel_surface_add(Ref<TerraSurface> value);
	void voxel_surface_set(const int index, Ref<TerraSurface> value);
	void voxel_surface_remove(const int index);
	int voxel_surface_get_num() const;
	void voxel_surfaces_clear();

	Vector<Variant> get_voxel_surfaces();
	void set_voxel_surfaces(const Vector<Variant> &surfaces);

	void refresh_rects();

	TerramanLibrarySimple();
	~TerramanLibrarySimple();

protected:
	static void _bind_methods();

private:
	Vector<Ref<TerraSurfaceSimple> > _voxel_surfaces;

	//atlas
	int _atlas_columns;
	int _atlas_rows;
};

#endif // TERRA_LIBRARY_H
