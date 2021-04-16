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

#ifndef TERRA_SURFACE_SIMPLE_H
#define TERRA_SURFACE_SIMPLE_H

#include "terra_surface.h"

class TerraSurfaceSimple : public TerraSurface {
	GDCLASS(TerraSurfaceSimple, TerraSurface)

public:
	int get_atlas_x(const TerraSurfaceSides side) const;
	void set_atlas_x(const TerraSurfaceSides side, int value);

	int get_atlas_y(const TerraSurfaceSides side) const;
	void set_atlas_y(const TerraSurfaceSides side, int value);

	void refresh_rects();

	TerraSurfaceSimple();
	~TerraSurfaceSimple();

protected:
	static void _bind_methods();

private:
	int _atlas_positions[TERRA_SIDES_ARRAY_SIZE];
};

#endif
