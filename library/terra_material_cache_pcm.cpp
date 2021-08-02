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

#include "../../texture_packer/texture_packer.h"
#include "scene/resources/texture.h"
#include "terra_surface.h"
#include "terra_surface_merger.h"

int TerraMaterialCachePCM::get_texture_flags() const {
	return _packer->get_texture_flags();
}
void TerraMaterialCachePCM::set_texture_flags(const int flags) {
	_packer->set_texture_flags(flags);
}

int TerraMaterialCachePCM::get_max_atlas_size() const {
	return _packer->get_max_atlas_size();
}
void TerraMaterialCachePCM::set_max_atlas_size(const int size) {
	_packer->set_max_atlas_size(size);
}

bool TerraMaterialCachePCM::get_keep_original_atlases() const {
	return _packer->get_keep_original_atlases();
}
void TerraMaterialCachePCM::set_keep_original_atlases(const bool value) {
	_packer->set_keep_original_atlases(value);
}

Color TerraMaterialCachePCM::get_background_color() const {
	return _packer->get_background_color();
}
void TerraMaterialCachePCM::set_background_color(const Color &color) {
	_packer->set_background_color(color);
}

int TerraMaterialCachePCM::get_margin() const {
	return _packer->get_margin();
}
void TerraMaterialCachePCM::set_margin(const int margin) {
	_packer->set_margin(margin);
}

void TerraMaterialCachePCM::refresh_rects() {
	bool texture_added = false;
	for (int i = 0; i < _surfaces.size(); i++) {
		Ref<TerraSurfaceMerger> surface = Ref<TerraSurfaceMerger>(_surfaces[i]);

		if (surface.is_valid()) {
			for (int j = 0; j < TerraSurface::TERRA_SIDES_COUNT; ++j) {
				Ref<Texture> tex = surface->get_texture(static_cast<TerraSurface::TerraSurfaceSides>(j));

				if (!tex.is_valid())
					continue;

				if (!_packer->contains_texture(tex)) {
					texture_added = true;
					surface->set_region(static_cast<TerraSurface::TerraSurfaceSides>(j), _packer->add_texture(tex));
				} else {
					surface->set_region(static_cast<TerraSurface::TerraSurfaceSides>(j), _packer->get_texture(tex));
				}
			}
		}
	}

	if (texture_added) {
		_packer->merge();

		ERR_FAIL_COND(_packer->get_texture_count() == 0);

		Ref<Texture> tex = _packer->get_generated_texture(0);

		setup_material_albedo(tex);
	}

	for (int i = 0; i < _surfaces.size(); i++) {
		Ref<TerraSurfaceMerger> surface = _surfaces[i];

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}
}

void TerraMaterialCachePCM::_setup_material_albedo(Ref<Texture> texture) {
	int count = material_get_num();

	for (int i = 0; i < count; ++i) {
		Ref<Material> m = material_get(i);

		Ref<SpatialMaterial> spmat = m;

		if (spmat.is_valid()) {
			spmat->set_texture(SpatialMaterial::TEXTURE_ALBEDO, texture);
			return;
		}

		Ref<ShaderMaterial> shmat = m;

		if (shmat.is_valid()) {
			shmat->set_shader_param("texture_albedo", texture);
		}
	}
}

TerraMaterialCachePCM::TerraMaterialCachePCM() {
	_packer.instance();

#if GODOT4
#warning implement
#else
	_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);
#endif

	_packer->set_max_atlas_size(1024);
	_packer->set_keep_original_atlases(false);
	_packer->set_margin(0);
}

TerraMaterialCachePCM::~TerraMaterialCachePCM() {
	for (int i = 0; i < _surfaces.size(); ++i) {
		Ref<TerraSurface> surface = _surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<TerramanLibrary>());
		}
	}

	_surfaces.clear();

	_packer->clear();
	_packer.unref();
}

void TerraMaterialCachePCM::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TerraMaterialCachePCM::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &TerraMaterialCachePCM::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &TerraMaterialCachePCM::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &TerraMaterialCachePCM::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &TerraMaterialCachePCM::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &TerraMaterialCachePCM::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &TerraMaterialCachePCM::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &TerraMaterialCachePCM::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &TerraMaterialCachePCM::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &TerraMaterialCachePCM::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");
}
