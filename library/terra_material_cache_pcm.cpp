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

#include "terra_material_cache_pcm.h"

#include "terra_surface.h"

//Surfaces
Ref<TerraSurface> TerraMaterialCachePCM::voxel_surface_get(const int index) {
	return Ref<TerraSurface>();
}
void TerraMaterialCachePCM::voxel_surface_add(Ref<TerraSurface> value) {
}
void TerraMaterialCachePCM::voxel_surface_set(int index, Ref<TerraSurface> value) {
}
void TerraMaterialCachePCM::voxel_surface_remove(const int index) {
}
int TerraMaterialCachePCM::voxel_surface_get_num() const {
	return 0;
}
void TerraMaterialCachePCM::voxel_surfaces_clear() {
}

void TerraMaterialCachePCM::refresh_rects() {
}

TerraMaterialCachePCM::TerraMaterialCachePCM() {
}

TerraMaterialCachePCM::~TerraMaterialCachePCM() {
}

void TerraMaterialCachePCM::_bind_methods() {
}
