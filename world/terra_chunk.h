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

#ifndef TERRA_CHUNK_H
#define TERRA_CHUNK_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#include "core/string/ustring.h"
#include "core/config/engine.h"
#include "core/variant/array.h"
#else
#include "core/resource.h"
#include "core/ustring.h"
#include "core/engine.h"
#include "core/array.h"
#endif

#include "../defines.h"

#include pool_vector_h
include_pool_vector

#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"

#include "scene/resources/packed_scene.h"

#include "terra_world.h"

#include "../data/terra_light.h"

#include "../meshers/terra_mesher.h"

#if PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

#include "core/version.h"

#if VERSION_MAJOR >= 4
#define Texture Texture2D
#endif

#include "../library/terra_surface.h"
#include "../library/terraman_library.h"
		; //hackfix for a clang format issue

class TerraJob;
class TerraWorld;
class TerraStructure;

class TerraChunk : public Resource {
	GDCLASS(TerraChunk, Resource);

	_THREAD_SAFE_CLASS_

public:
	enum {
		TERRA_CHUNK_STATE_OK = 0,
	};

public:
	bool get_is_build_threaded() const;
	void set_is_build_threaded(const bool value);

	bool get_process() const;
	void set_process(const bool value);

	bool get_physics_process() const;
	void set_physics_process(const bool value);

	bool get_visible() const;
	void set_visible(const bool value);

	bool get_is_generating() const;
	void set_is_generating(const bool value);

	bool is_in_tree() const;

	bool get_dirty() const;
	void set_dirty(const bool value);

	int get_state() const;
	void set_state(int value);

	int get_position_x() const;
	void set_position_x(const int value);
	int get_position_z() const;
	void set_position_z(const int value);

	int get_size_x() const;
	int get_size_z() const;
	void set_size_x(const int value);
	void set_size_z(const int value);

	int get_data_size_x() const;
	int get_data_size_z() const;
	void set_data_size_x(const int value);
	void set_data_size_z(const int value);

	float get_world_height() const;
	void set_world_height(const float value);

	Vector2 get_position() const;
	Vector3 get_size() const;

	Vector2 get_world_position() const;
	Vector3 get_world_size() const;
	AABB get_world_aabb() const;

	void set_position(const int x, const int z);

	int get_margin_start() const;
	int get_margin_end() const;
	void set_margin_start(const int value);
	void set_margin_end(const int value);

	Ref<TerramanLibrary> get_library();
	void set_library(const Ref<TerramanLibrary> &value);

	float get_voxel_scale() const;
	void set_voxel_scale(const float value);

	TerraWorld *get_voxel_world() const;
	void set_voxel_world(TerraWorld *world);
	void set_voxel_world_bind(Node *world);

	//Jobs
	Ref<TerraJob> job_get(const int index) const;
	void job_set(const int index, const Ref<TerraJob> &job);
	void job_remove(const int index);
	void job_add(const Ref<TerraJob> &job);
	int job_get_count() const;

	int job_get_current_index();
	void job_next();
	Ref<TerraJob> job_get_current();

	//Channels
	void channel_setup();

	void set_size(const int size_x, const int size_z, const int margin_start = 0, const int margin_end = 0);

	bool validate_data_position(const int x, const int z) const;

	uint8_t get_voxel(const int p_x, const int p_z, const int p_index) const;
	void set_voxel(const uint8_t p_value, const int p_x, const int p_z, const int p_index);

	int channel_get_count() const;
	void channel_set_count(const int count);

	bool channel_is_allocated(const int channel_index);
	void channel_ensure_allocated(const int channel_index, const uint8_t default_value = 0);
	void channel_allocate(const int channel_index, const uint8_t default_value = 0);
	void channel_fill(const uint8_t value, const int channel_index);
	void channel_dealloc(const int channel_index);

	uint8_t *channel_get(const int channel_index);
	uint8_t *channel_get_valid(const int channel_index, const uint8_t default_value = 0);

	PoolByteArray channel_get_array(const int channel_index) const;
	void channel_set_array(const int channel_index, const PoolByteArray &array);

	PoolByteArray channel_get_compressed(const int channel_index) const;
	void channel_set_compressed(const int channel_index, const PoolByteArray &data);

	int get_index(const int x, const int z) const;
	int get_data_index(const int x, const int z) const;
	int get_data_size() const;

	//Terra Structures
	Ref<TerraStructure> voxel_structure_get(const int index) const;
	void voxel_structure_add(const Ref<TerraStructure> &structure);
	void voxel_structure_remove(const Ref<TerraStructure> &structure);
	void voxel_structure_remove_index(const int index);
	void voxel_structure_clear();
	int voxel_structure_get_count() const;
	void voxel_structure_add_at_position(Ref<TerraStructure> structure, const Vector3 &world_position);

	Vector<Variant> voxel_structures_get();
	void voxel_structures_set(const Vector<Variant> &structures);

	//Meshing
	void build();
	void clear();
	void finalize_build();

	void _build();

	//light Baking
	void bake_lights();
	void bake_light(Ref<TerraLight> light);
	void clear_baked_lights();

#if PROPS_PRESENT
	void prop_add(const Transform &tarnsform, const Ref<PropData> &prop);
	Ref<PropData> prop_get(const int index);
	Transform prop_get_tarnsform(const int index);
	int prop_get_count() const;
	void prop_remove(const int index);
	void props_clear();
#endif

#if MESH_DATA_RESOURCE_PRESENT
	int mesh_data_resource_addv(const Vector3 &local_data_pos, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture = Ref<Texture>(), const Color &color = Color(1, 1, 1, 1), const bool apply_voxel_scale = true);
	int mesh_data_resource_add(const Transform &local_transform, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture = Ref<Texture>(), const Color &color = Color(1, 1, 1, 1), const bool apply_voxel_scale = true);

	Ref<MeshDataResource> mesh_data_resource_get(const int index);
	void mesh_data_resource_set(const int index, const Ref<MeshDataResource> &mesh);

	Ref<Texture> mesh_data_resource_get_texture(const int index);
	void mesh_data_resource_set_texture(const int index, const Ref<Texture> &texture);

	Color mesh_data_resource_get_color(const int index);
	void mesh_data_resource_set_color(const int index, const Color &color);

	Rect2 mesh_data_resource_get_uv_rect(const int index);
	void mesh_data_resource_set_uv_rect(const int index, const Rect2 &uv_rect);

	Transform mesh_data_resource_get_transform(const int index);
	void mesh_data_resource_set_transform(const int index, const Transform &transform);

	bool mesh_data_resource_get_is_inside(const int index);
	void mesh_data_resource_set_is_inside(const int index, const bool &inside);

	int mesh_data_resource_get_count() const;
	void mesh_data_resource_remove(const int index);
	void mesh_data_resource_clear();
#endif

	//Colliders
	int collider_add(const Transform &local_transform, const Ref<Shape> &shape, const RID &shape_rid = RID(), const RID &body = RID());

	Transform collider_get_transform(const int index);
	void collider_set_transform(const int index, const Transform &transform);

	Ref<Shape> collider_get_shape(const int index);
	void collider_set_shape(const int index, const Ref<Shape> &shape);

	RID collider_get_shape_rid(const int index);
	void collider_set_shape_rid(const int index, const RID &rid);

	RID collider_get_body(const int index);
	void collider_set_body(const int index, const RID &rid);

	int collider_get_count() const;
	void collider_remove(const int index);
	void colliders_clear();

	//handlers
	void enter_tree();
	void exit_tree();
	void process(const float delta);
	void physics_process(const float delta);
	void world_transform_changed();
	void visibility_changed(const bool visible);
	void world_light_added(const Ref<TerraLight> &light);
	void world_light_removed(const Ref<TerraLight> &light);
	void generation_process(const float delta);
	void generation_physics_process(const float delta);

	Transform get_transform() const;
	void set_transform(const Transform &transform);

	Transform get_global_transform() const;
	Vector3 to_local(Vector3 p_global) const;
	Vector3 to_global(Vector3 p_local) const;

	TerraChunk();
	~TerraChunk();

protected:
	virtual void _enter_tree();
	virtual void _exit_tree();
	virtual void _generation_process(const float delta);
	virtual void _generation_physics_process(const float delta);

protected:
#if PROPS_PRESENT
	struct PropDataStore {
		Transform transform;
		Ref<PropData> prop;
	};
#endif

#if MESH_DATA_RESOURCE_PRESENT
	struct MeshDataResourceEntry {
		Ref<MeshDataResource> mesh;
		Ref<Texture> texture;
		Color color;
		Rect2 uv_rect;
		Transform transform;
		bool is_inside;
	};
#endif

	struct ColliderBody {
		Transform transform;
		RID body;
		Ref<Shape> shape;
		RID shape_rid;
	};

protected:
	virtual void _world_transform_changed();

	/*
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	*/
	static void _bind_methods();

	bool _is_build_threaded;

	bool _is_processing;
	bool _is_phisics_processing;

	bool _is_visible;

	bool _is_generating;
	bool _dirty;
	int _state;

	bool _is_in_tree;

	TerraWorld *_voxel_world;

	int _position_x;
	int _position_z;

	int _size_x;
	int _size_z;

	int _data_size_x;
	int _data_size_z;

	int _margin_start;
	int _margin_end;

	float _world_height;

	Vector<uint8_t *> _channels;

	float _voxel_scale;

	int _current_job;
	Vector<Ref<TerraJob> > _jobs;

	Ref<TerramanLibrary> _library;

	Vector<Ref<TerraStructure> > _voxel_structures;

#if PROPS_PRESENT
	Vector<PropDataStore> _props;
#endif

#if MESH_DATA_RESOURCE_PRESENT
	Vector<MeshDataResourceEntry> _mesh_data_resources;
#endif

	Vector<ColliderBody> _colliders;

	Transform _transform;

	bool _abort_build;
	bool _queued_generation;
};

#endif
