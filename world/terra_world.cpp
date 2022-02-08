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

#include "terra_world.h"

#include "core/version.h"

#include "terra_chunk.h"
#include "terra_structure.h"

#include "../defines.h"

#if PROPS_PRESENT
#include "../../props/props/prop_data.h"
#include "../../props/props/prop_data_entry.h"
#include "../../props/props/prop_data_light.h"
#include "../../props/props/prop_data_prop.h"
#include "../../props/props/prop_data_scene.h"
#endif

#if MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif

const String TerraWorld::BINDING_STRING_CHANNEL_TYPE_INFO = "Type,Isolevel,Liquid,Liquid Level";

bool TerraWorld::get_active() const {
	return _active;
}
void TerraWorld::set_active(const bool value) {
	_active = value;

	notification(NOTIFICATION_ACTIVE_STATE_CHANGED);
}

bool TerraWorld::get_editable() const {
	return _editable;
}
void TerraWorld::set_editable(const bool value) {
	_editable = value;
}

int TerraWorld::get_chunk_size_x() const {
	return _chunk_size_x;
}
void TerraWorld::set_chunk_size_x(const int value) {
	_chunk_size_x = value;
}

int TerraWorld::get_chunk_size_z() const {
	return _chunk_size_z;
}
void TerraWorld::set_chunk_size_z(const int value) {
	_chunk_size_z = value;
}

int TerraWorld::get_data_margin_start() const {
	return _data_margin_start;
}
void TerraWorld::set_data_margin_start(const int value) {
	_data_margin_start = value;
}

int TerraWorld::get_data_margin_end() const {
	return _data_margin_end;
}
void TerraWorld::set_data_margin_end(const int value) {
	_data_margin_end = value;
}

int TerraWorld::get_current_seed() const {
	return _current_seed;
}
void TerraWorld::set_current_seed(const int value) {
	_current_seed = value;
}

_FORCE_INLINE_ float TerraWorld::get_world_height() const {
	return _world_height;
}
void TerraWorld::set_world_height(const float value) {
	_world_height = value;
}

int TerraWorld::get_max_concurrent_generations() {
	return _max_concurrent_generations;
}
void TerraWorld::set_max_concurrent_generations(const int value) {
	_max_concurrent_generations = value;
}

Ref<TerramanLibrary> TerraWorld::get_library() {
	return _library;
}
void TerraWorld::set_library(const Ref<TerramanLibrary> &library) {
	_library = library;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<TerraChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_library(_library);
	}
}

Ref<TerramanLevelGenerator> TerraWorld::get_level_generator() const {
	return _level_generator;
}
void TerraWorld::set_level_generator(const Ref<TerramanLevelGenerator> &level_generator) {
	_level_generator = level_generator;
}

float TerraWorld::get_voxel_scale() const {
	return _voxel_scale;
}
void TerraWorld::set_voxel_scale(const float value) {
	_voxel_scale = value;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<TerraChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_voxel_scale(_voxel_scale);
	}
}

int TerraWorld::get_chunk_spawn_range() const {
	return _chunk_spawn_range;
}
void TerraWorld::set_chunk_spawn_range(const int value) {
	_chunk_spawn_range = value;
}

NodePath TerraWorld::get_player_path() const {
	return _player_path;
}

void TerraWorld::set_player_path(const NodePath &player_path) {
	_player_path = player_path;
}

Spatial *TerraWorld::get_player() const {
	return _player;
}
void TerraWorld::set_player(Spatial *player) {
	_player = player;
}
void TerraWorld::set_player_bind(Node *player) {
	set_player(Object::cast_to<Spatial>(player));
}

Ref<TerraWorldArea> TerraWorld::world_area_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _world_areas.size(), Ref<TerraWorldArea>());

	return _world_areas.get(index);
}
void TerraWorld::world_area_add(const Ref<TerraWorldArea> &area) {
	_world_areas.push_back(area);
}
void TerraWorld::world_area_remove(const int index) {
	ERR_FAIL_INDEX(index, _world_areas.size());

	_world_areas.VREMOVE(index);
}
void TerraWorld::world_areas_clear() {
	_world_areas.clear();
}
int TerraWorld::world_area_get_count() const {
	return _world_areas.size();
}

//Terra Structures

Ref<TerraStructure> TerraWorld::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<TerraStructure>());

	return _voxel_structures.get(index);
}
void TerraWorld::voxel_structure_add(const Ref<TerraStructure> &structure) {
	_voxel_structures.push_back(structure);
}
void TerraWorld::voxel_structure_remove(const Ref<TerraStructure> &structure) {
	if (!structure.is_valid())
		return;

	int index = _voxel_structures.find(structure);

	if (index != -1)
		_voxel_structures.VREMOVE(index);
}
void TerraWorld::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

	_voxel_structures.VREMOVE(index);
}
void TerraWorld::voxel_structures_clear() {
	_voxel_structures.clear();
}
int TerraWorld::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void TerraWorld::voxel_structure_add_at_position(Ref<TerraStructure> structure, const Vector3 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _voxel_scale));
	structure->set_position_y(static_cast<int>(world_position.y / _voxel_scale));
	structure->set_position_z(static_cast<int>(world_position.z / _voxel_scale));

	voxel_structure_add(structure);
}

Vector<Variant> TerraWorld::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void TerraWorld::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structures_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<TerraLight> structure = Ref<TerraLight>(structures[i]);

		voxel_structure_add(structure);
	}
}

void TerraWorld::chunk_add(Ref<TerraChunk> chunk, const int x, const int z) {
	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND_MSG(chunk->get_voxel_world() != NULL && chunk->get_voxel_world() != this, "Chunk is already owned by an another world!");

	IntPos pos(x, z);

	//ERR_FAIL_COND(_chunks.has(pos));

	chunk->set_voxel_world(this);
	chunk->set_position(x, z);
	chunk->world_transform_changed();

	if (!_chunks.has(pos))
		_chunks.set(pos, chunk);

	if (_chunks_vector.find(chunk) == -1)
		_chunks_vector.push_back(chunk);

	if (is_inside_tree())
		chunk->enter_tree();

	if (has_method("_chunk_added")) {
		CALL(_chunk_added, chunk);
	}

	emit_signal("chunk_added", chunk);
}
bool TerraWorld::chunk_has(const int x, const int z) const {
	return _chunks.has(IntPos(x, z));
}
Ref<TerraChunk> TerraWorld::chunk_get(const int x, const int z) {
	IntPos pos(x, z);

	if (_chunks.has(pos))
		return _chunks.get(pos);

	return Ref<TerraChunk>();
}
Ref<TerraChunk> TerraWorld::chunk_remove(const int x, const int z) {
	IntPos pos(x, z);

	if (!_chunks.has(pos))
		return NULL;

	Ref<TerraChunk> chunk = _chunks.get(pos);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		if (_chunks_vector.get(i) == chunk) {
			_chunks_vector.VREMOVE(i);
			break;
		}
	}

	chunk->exit_tree();

	_chunks.erase(pos);

	emit_signal("chunk_removed", chunk);

	return chunk;
}
Ref<TerraChunk> TerraWorld::chunk_remove_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	Ref<TerraChunk> chunk = _chunks_vector.get(index);
	_chunks_vector.VREMOVE(index);
	_chunks.erase(IntPos(chunk->get_position_x(), chunk->get_position_z()));
	chunk->exit_tree();

	emit_signal("chunk_removed", chunk);

	return chunk;
}

Ref<TerraChunk> TerraWorld::chunk_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	return _chunks_vector.get(index);
}
int TerraWorld::chunk_get_count() const {
	return _chunks_vector.size();
}

void TerraWorld::chunks_clear() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<TerraChunk> chunk = _chunks_vector.get(i);

		chunk->exit_tree();

		emit_signal("chunk_removed", chunk);
	}

	_chunks_vector.clear();

	_chunks.clear();

	_generation_queue.clear();
	_generating.clear();
}

Ref<TerraChunk> TerraWorld::chunk_get_or_create(int x, int z) {
	Ref<TerraChunk> chunk = chunk_get(x, z);

	if (!chunk.is_valid()) {
		chunk = chunk_create(x, z);
	}

	return chunk;
}

Ref<TerraChunk> TerraWorld::chunk_create(const int x, const int z) {
	Ref<TerraChunk> c;
	GET_CALLP(Ref<TerraChunk>, c, _create_chunk, x, z, Ref<TerraChunk>());

	generation_queue_add_to(c);

	return c;
}

void TerraWorld::chunk_setup(Ref<TerraChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

#if VERSION_MAJOR < 4
	call("_create_chunk", chunk->get_position_x(), chunk->get_position_z(), chunk);
#else
	Ref<TerraChunk> c;
	GDVIRTUAL_CALL(_create_chunk, chunk->get_position_x(), chunk->get_position_z(), chunk, c);
#endif
}

Ref<TerraChunk> TerraWorld::_create_chunk(const int x, const int z, Ref<TerraChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk.INSTANCE();
	}

	//no meshers here

	ERR_FAIL_COND_V(!chunk.is_valid(), NULL);

	chunk->set_name("Chunk[" + String::num(x) + "," + String::num(z) + "]");

	chunk->set_voxel_world(this);

	chunk->set_position(x, z);
	chunk->set_world_height(_world_height);
	chunk->set_library(_library);
	chunk->set_voxel_scale(_voxel_scale);
	chunk->set_size(_chunk_size_x, _chunk_size_z, _data_margin_start, _data_margin_end);
	//chunk->set_translation(Vector3(x * _chunk_size_x * _voxel_scale, y * _chunk_size_y * _voxel_scale, z * _chunk_size_z * _voxel_scale));

	if (!get_active()) {
		chunk->set_visible(false);
	}

	chunk_add(chunk, x, z);

	return chunk;
}

void TerraWorld::chunk_generate(Ref<TerraChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_prepare_chunk_for_generation")) {
		CALL(_prepare_chunk_for_generation, chunk);
	}

	CALL(_generate_chunk, chunk);

	chunk->build();
}

Vector<Variant> TerraWorld::chunks_get() {
	VARIANT_ARRAY_GET(_chunks_vector);
}

void TerraWorld::chunks_set(const Vector<Variant> &chunks) {
	if (is_inside_tree()) {
		for (int i = 0; i < _chunks_vector.size(); ++i) {
			Ref<TerraChunk> chunk = Ref<TerraChunk>(_chunks_vector[i]);

			if (chunks.find(chunk) == -1) {
				chunk_remove_index(i);
				_generation_queue.erase(chunk);
				_generating.erase(chunk);
				--i;
			}
		}

		//add the difference
		for (int i = 0; i < chunks.size(); ++i) {
			Ref<TerraChunk> chunk = Ref<TerraChunk>(chunks[i]);

			if (!chunk.is_valid())
				continue;

			if (_chunks_vector.find(chunk) != -1)
				continue;

			chunk_add(chunk, chunk->get_position_x(), chunk->get_position_z());
		}
	} else {
		_chunks_vector.clear();

		for (int i = 0; i < chunks.size(); ++i) {
			Ref<TerraChunk> chunk = Ref<TerraChunk>(chunks[i]);

			_chunks_vector.push_back(chunk);
		}
	}
}

int TerraWorld::_get_channel_index_info(const TerraWorld::ChannelTypeInfo channel_type) {
	return -1;
}

void TerraWorld::_set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel) {
	Vector3 pos;

	if (mode_add) {
		pos = (hit_position + (Vector3(0.1, 0.1, 0.1) * hit_normal * get_voxel_scale()));
	} else {
		pos = (hit_position + (Vector3(0.1, 0.1, 0.1) * -hit_normal * get_voxel_scale()));
	}

	int channel_type = get_channel_index_info(TerraWorld::CHANNEL_TYPE_INFO_TYPE);
	int channel_isolevel = get_channel_index_info(TerraWorld::CHANNEL_TYPE_INFO_ISOLEVEL);

	if (channel_isolevel == -1) {
		set_voxel_at_world_position(pos, selected_voxel, channel_type);
	} else {
		set_voxel_at_world_position(pos, selected_voxel, channel_type, false);
		set_voxel_at_world_position(pos, isolevel, channel_isolevel);
	}
}

bool TerraWorld::can_chunk_do_build_step() {
	if (_max_frame_chunk_build_steps == 0) {
		return true;
	}

	return _num_frame_chunk_build_steps++ < _max_frame_chunk_build_steps;
}

bool TerraWorld::is_position_walkable(const Vector3 &p_pos) {
	int x = static_cast<int>(Math::floor(p_pos.x / (_chunk_size_x * _voxel_scale)));
	int z = static_cast<int>(Math::floor(p_pos.z / (_chunk_size_z * _voxel_scale)));

	Ref<TerraChunk> c = chunk_get(x, z);

	if (!c.is_valid())
		return false;

	return !c->get_is_generating();
}

void TerraWorld::on_chunk_mesh_generation_finished(Ref<TerraChunk> p_chunk) {
	call_deferred("emit_signal", "chunk_mesh_generation_finished", p_chunk);
}

void TerraWorld::generation_queue_add_to(const Ref<TerraChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	set_process_internal(true);

	_generation_queue.push_back(chunk);
}
Ref<TerraChunk> TerraWorld::generation_queue_get_index(int index) {
	ERR_FAIL_INDEX_V(index, _generation_queue.size(), NULL);

	return _generation_queue.get(index);
}
void TerraWorld::generation_queue_remove_index(int index) {
	ERR_FAIL_INDEX(index, _generation_queue.size());

	_generation_queue.VREMOVE(index);
}
int TerraWorld::generation_queue_get_size() const {
	return _generation_queue.size();
}

void TerraWorld::generation_add_to(const Ref<TerraChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	_generating.push_back(chunk);
}
Ref<TerraChunk> TerraWorld::generation_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _generating.size(), NULL);

	return _generating.get(index);
}
void TerraWorld::generation_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _generating.size());

	_generating.VREMOVE(index);
}
int TerraWorld::generation_get_size() const {
	return _generating.size();
}

#if PROPS_PRESENT
void TerraWorld::prop_add(Transform transform, const Ref<PropData> &prop, const bool apply_voxel_scale) {
	ERR_FAIL_COND(!prop.is_valid());

	if (apply_voxel_scale) {
		transform = transform.scaled(Vector3(get_voxel_scale(), get_voxel_scale(), get_voxel_scale()));
	}

	Vector3 wp;
	wp = transform.xform(wp);
	Ref<TerraChunk> chunk = get_or_create_chunk_at_world_position(wp);

	chunk->prop_add(transform, prop);

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<PropDataEntry> entry = prop->get_prop(i);

		if (!entry.is_valid())
			continue;

		Transform t = transform * entry->get_transform();

		wp = t.xform(Vector3());
		chunk = get_or_create_chunk_at_world_position(wp);

		Ref<PropDataProp> prop_entry_data = entry;

		if (prop_entry_data.is_valid()) {
			Ref<PropData> p = prop_entry_data->get_prop();

			if (!p.is_valid())
				continue;

			prop_add(t, p, false);

			continue;
		}

		Ref<PropDataScene> scene_data = entry;

		if (scene_data.is_valid()) {
			Ref<PackedScene> sc = scene_data->get_scene();

			if (!sc.is_valid())
				continue;

			Node *n = sc->INSTANCE();
			add_child(n);
			n->set_owner(this);

			Spatial *sp = Object::cast_to<Spatial>(n);

			if (sp) {
				sp->set_transform(t);
			}

			continue;
		}

		Ref<PropDataLight> light_data = entry;

		if (light_data.is_valid()) {
			Ref<TerraLight> light;
			light.INSTANCE();

			light->set_world_position(wp.x / get_voxel_scale(), wp.y / get_voxel_scale(), wp.z / get_voxel_scale());
			light->set_color(light_data->get_light_color());
			light->set_size(light_data->get_light_size());

			light_add(light);

			continue;
		}

#if MESH_DATA_RESOURCE_PRESENT
		Ref<PropDataMeshData> mesh_data = entry;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid())
				continue;

			Transform chunk_local_tform = t;

			chunk_local_tform.origin = chunk->to_local(chunk_local_tform.origin);

			chunk->mesh_data_resource_add(chunk_local_tform, mdr, mesh_data->get_texture(), Color(1, 1, 1, 1), false);

			continue;
		}
#endif
	}
}
#endif

//Lights
void TerraWorld::light_add(const Ref<TerraLight> &light) {
	_lights.push_back(light);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<TerraChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_added(light);
		}
	}
}
Ref<TerraLight> TerraWorld::light_get(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<TerraLight>());

	return _lights.get(index);
}
void TerraWorld::light_remove(const int index) {
	ERR_FAIL_INDEX(index, _lights.size());

	Ref<TerraLight> light = _lights[index];

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<TerraChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_removed(light);
		}
	}
}
int TerraWorld::light_get_count() const {
	return _lights.size();
}
void TerraWorld::lights_clear() {
	for (int i = 0; i < _lights.size(); ++i) {
		Ref<TerraLight> light = _lights[i];

		if (!light.is_valid())
			continue;

		for (int j = 0; j < _chunks_vector.size(); ++j) {
			Ref<TerraChunk> chunk = _chunks_vector[j];

			if (chunk.is_valid()) {
				chunk->world_light_removed(light);
			}
		}
	}

	_lights.clear();
}

Vector<Variant> TerraWorld::lights_get() {
	VARIANT_ARRAY_GET(_lights);
}
void TerraWorld::lights_set(const Vector<Variant> &chunks) {
	lights_clear();

	for (int i = 0; i < chunks.size(); ++i) {
		Ref<TerraLight> light = Ref<TerraLight>(chunks[i]);

		light_add(light);
	}
}

uint8_t TerraWorld::get_voxel_at_world_position(const Vector3 &world_position, const int channel_index) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int bz = static_cast<int>(Math::floor(pos.z)) % get_chunk_size_z();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (bz < 0) {
		bz += get_chunk_size_z();
	}

	Ref<TerraChunk> chunk = chunk_get(x, z);

	if (chunk.is_valid())
		return chunk->get_voxel(bx, bz, channel_index);

	return 0;
}

void TerraWorld::set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index, const bool rebuild) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers properly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
	int bz = static_cast<int>(Math::floor(pos.z)) % get_chunk_size_z();

	if (bx < 0) {
		bx += get_chunk_size_x();
	}

	if (bz < 0) {
		bz += get_chunk_size_z();
	}

	if (get_data_margin_end() > 0) {
		if (bx == 0) {
			Ref<TerraChunk> chunk = chunk_get_or_create(x - 1, z);
			chunk->set_voxel(data, get_chunk_size_x(), bz, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (bz == 0) {
			Ref<TerraChunk> chunk = chunk_get_or_create(x, z - 1);
			chunk->set_voxel(data, bx, get_chunk_size_z(), channel_index);

			if (rebuild)
				chunk->build();
		}
	}

	if (get_data_margin_start() > 0) {
		if (bx == get_chunk_size_x() - 1) {
			Ref<TerraChunk> chunk = chunk_get_or_create(x + 1, z);
			chunk->set_voxel(data, -1, bz, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (bz == get_chunk_size_z() - 1) {
			Ref<TerraChunk> chunk = chunk_get_or_create(x, z + 1);
			chunk->set_voxel(data, bx, -1, channel_index);

			if (rebuild)
				chunk->build();
		}
	}

	Ref<TerraChunk> chunk = chunk_get_or_create(x, z);
	chunk->set_voxel(data, bx, bz, channel_index);

	if (rebuild)
		chunk->build();
}

Ref<TerraChunk> TerraWorld::get_chunk_at_world_position(const Vector3 &world_position) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	return chunk_get(x, z);
}
Ref<TerraChunk> TerraWorld::get_or_create_chunk_at_world_position(const Vector3 &world_position) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	return chunk_get_or_create(x, z);
}

void TerraWorld::set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel) {
	CALL(_set_voxel_with_tool, mode_add, hit_position, hit_normal, selected_voxel, isolevel);
}

int TerraWorld::get_channel_index_info(const TerraWorld::ChannelTypeInfo channel_type) {
	RETURN_CALLP(int, _get_channel_index_info, channel_type);
}

TerraWorld::TerraWorld() {
	_active = true;
	_editable = false;

	_is_priority_generation = true;
	_max_concurrent_generations = 3;

	_chunk_size_x = 16;
	_chunk_size_z = 16;
	_current_seed = 0;
	_data_margin_start = 0;
	_data_margin_end = 0;
	_world_height = 256;

	_voxel_scale = 1;
	_chunk_spawn_range = 4;

	_player = NULL;
	_max_frame_chunk_build_steps = 0;
	_num_frame_chunk_build_steps = 0;
}

TerraWorld ::~TerraWorld() {
	_chunks.clear();
	_chunks_vector.clear();
	_world_areas.clear();
	_voxel_structures.clear();

	_library.unref();
	_level_generator.unref();

	_player = NULL;

	_generation_queue.clear();
	_generating.clear();

	_lights.clear();
}

void TerraWorld::_generate_chunk(Ref<TerraChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (_level_generator.is_valid())
		_level_generator->generate_chunk(chunk);

	for (int i = 0; i < _voxel_structures.size(); ++i) {
		Ref<TerraStructure> structure = _voxel_structures.get(i);

		if (!structure.is_valid())
			continue;

		if (structure->get_use_aabb()) {
			if (structure->get_chunk_aabb().has_point(Vector3(chunk->get_position_x(), 1, chunk->get_position_z())))
				structure->write_to_chunk(chunk);
		} else {
			structure->write_to_chunk(chunk);
		}
	}
}

void TerraWorld::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_player_bind(get_node_or_null(get_player_path()));

			set_process_internal(true);
			set_physics_process_internal(true);
			set_notify_transform(true);

			if (_library.is_valid())
				_library->refresh_rects();

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerraChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk_setup(chunk);

					chunk->build();
				}
			}
			break;
		}
		case NOTIFICATION_INTERNAL_PROCESS: {
			_num_frame_chunk_build_steps = 0;

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerraChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->process(get_process_delta_time());
				}

				if (chunk->get_is_generating()) {
					chunk->generation_process(get_process_delta_time());
				}
			}

#if VERSION_MAJOR > 3
			if (_is_priority_generation && _generation_queue.is_empty() && _generating.is_empty()) {
#else
			if (_is_priority_generation && _generation_queue.empty() && _generating.empty()) {
#endif
				_is_priority_generation = false;

				CALL(_generation_finished);

				emit_signal("generation_finished");

				return;
			}

			for (int i = 0; i < _generating.size(); ++i) {
				Ref<TerraChunk> chunk = _generating.get(i);

				if (!chunk.is_valid() || !chunk->get_is_generating()) {
					_generating.VREMOVE(i);
					--i;
					continue;
				}
			}

			if (_generating.size() >= _max_concurrent_generations)
				return;

			if (_generation_queue.size() == 0)
				return;

			while (_generating.size() < _max_concurrent_generations && _generation_queue.size() != 0) {
				Ref<TerraChunk> chunk = _generation_queue.get(0);
				_generation_queue.VREMOVE(0);

				ERR_FAIL_COND(!chunk.is_valid());

				_generating.push_back(chunk);

				chunk_generate(chunk);
			}
			break;
		}
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerraChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->physics_process(get_physics_process_delta_time());
				}

				if (chunk->get_is_generating()) {
					chunk->generation_physics_process(get_physics_process_delta_time());
				}
			}
			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerraChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					if (chunk->get_voxel_world() == this) {
						chunk->exit_tree();
						chunk->set_voxel_world(NULL);
					}
				}
			}
			break;
		}
		case NOTIFICATION_TRANSFORM_CHANGED: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<TerraChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk->world_transform_changed();
				}
			}
			break;
		}
	}
}

void TerraWorld::_bind_methods() {
	ADD_SIGNAL(MethodInfo("chunk_mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
	ADD_SIGNAL(MethodInfo("chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
	ADD_SIGNAL(MethodInfo("chunk_removed", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));

	ClassDB::bind_method(D_METHOD("get_active"), &TerraWorld::get_active);
	ClassDB::bind_method(D_METHOD("set_active", "value"), &TerraWorld::set_active);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

	ClassDB::bind_method(D_METHOD("get_editable"), &TerraWorld::get_editable);
	ClassDB::bind_method(D_METHOD("set_editable", "value"), &TerraWorld::set_editable);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "editable"), "set_editable", "get_editable");

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &TerraWorld::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &TerraWorld::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &TerraWorld::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &TerraWorld::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("get_data_margin_start"), &TerraWorld::get_data_margin_start);
	ClassDB::bind_method(D_METHOD("set_data_margin_start", "value"), &TerraWorld::set_data_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_start"), "set_data_margin_start", "get_data_margin_start");

	ClassDB::bind_method(D_METHOD("get_data_margin_end"), &TerraWorld::get_data_margin_end);
	ClassDB::bind_method(D_METHOD("set_data_margin_end", "value"), &TerraWorld::set_data_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_end"), "set_data_margin_end", "get_data_margin_end");

	ClassDB::bind_method(D_METHOD("get_world_height"), &TerraWorld::get_world_height);
	ClassDB::bind_method(D_METHOD("set_world_height", "height"), &TerraWorld::set_world_height);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "world_height"), "set_world_height", "get_world_height");

	ClassDB::bind_method(D_METHOD("get_max_concurrent_generations"), &TerraWorld::get_max_concurrent_generations);
	ClassDB::bind_method(D_METHOD("set_max_concurrent_generations", "height"), &TerraWorld::set_max_concurrent_generations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_concurrent_generations"), "set_max_concurrent_generations", "get_max_concurrent_generations");

	ClassDB::bind_method(D_METHOD("get_current_seed"), &TerraWorld::get_current_seed);
	ClassDB::bind_method(D_METHOD("set_current_seed", "value"), &TerraWorld::set_current_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_seed"), "set_current_seed", "get_current_seed");

	ClassDB::bind_method(D_METHOD("get_library"), &TerraWorld::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "library"), &TerraWorld::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "TerramanLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_level_generator"), &TerraWorld::get_level_generator);
	ClassDB::bind_method(D_METHOD("set_level_generator", "level_generator"), &TerraWorld::set_level_generator);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "level_generator", PROPERTY_HINT_RESOURCE_TYPE, "TerramanLevelGenerator"), "set_level_generator", "get_level_generator");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &TerraWorld::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &TerraWorld::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_chunk_spawn_range"), &TerraWorld::get_chunk_spawn_range);
	ClassDB::bind_method(D_METHOD("set_chunk_spawn_range", "value"), &TerraWorld::set_chunk_spawn_range);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_spawn_range"), "set_chunk_spawn_range", "get_chunk_spawn_range");

	ClassDB::bind_method(D_METHOD("get_player_path"), &TerraWorld::get_player_path);
	ClassDB::bind_method(D_METHOD("set_player_path", "value"), &TerraWorld::set_player_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player_path"), "set_player_path", "get_player_path");

	ClassDB::bind_method(D_METHOD("get_player"), &TerraWorld::get_player);
	ClassDB::bind_method(D_METHOD("set_player", "player"), &TerraWorld::set_player_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_RESOURCE_TYPE, "Spatial", 0), "set_player", "get_player");

	ClassDB::bind_method(D_METHOD("world_area_get", "index"), &TerraWorld::world_area_get);
	ClassDB::bind_method(D_METHOD("world_area_add", "area"), &TerraWorld::world_area_add);
	ClassDB::bind_method(D_METHOD("world_area_remove", "index"), &TerraWorld::world_area_remove);
	ClassDB::bind_method(D_METHOD("world_areas_clear"), &TerraWorld::world_areas_clear);
	ClassDB::bind_method(D_METHOD("world_area_get_count"), &TerraWorld::world_area_get_count);

	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &TerraWorld::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &TerraWorld::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &TerraWorld::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &TerraWorld::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structures_clear"), &TerraWorld::voxel_structures_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &TerraWorld::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &TerraWorld::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &TerraWorld::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &TerraWorld::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "17/17:TerraStructure", PROPERTY_USAGE_DEFAULT, "TerraStructure"), "voxel_structures_set", "voxel_structures_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
#else
	GDVIRTUAL_BIND(_chunk_added, "chunk");
#endif

	ClassDB::bind_method(D_METHOD("chunk_add", "chunk", "x", "z"), &TerraWorld::chunk_add);
	ClassDB::bind_method(D_METHOD("chunk_has", "x", "z"), &TerraWorld::chunk_has);
	ClassDB::bind_method(D_METHOD("chunk_get", "x", "z"), &TerraWorld::chunk_get);
	ClassDB::bind_method(D_METHOD("chunk_remove", "x", "z"), &TerraWorld::chunk_remove);
	ClassDB::bind_method(D_METHOD("chunk_remove_index", "index"), &TerraWorld::chunk_remove_index);

	ClassDB::bind_method(D_METHOD("chunk_get_index", "index"), &TerraWorld::chunk_get_index);
	ClassDB::bind_method(D_METHOD("chunk_get_count"), &TerraWorld::chunk_get_count);

	ClassDB::bind_method(D_METHOD("chunks_clear"), &TerraWorld::chunks_clear);

	ClassDB::bind_method(D_METHOD("chunks_get"), &TerraWorld::chunks_get);
	ClassDB::bind_method(D_METHOD("chunks_set"), &TerraWorld::chunks_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "chunks", PROPERTY_HINT_NONE, "17/17:TerraChunk", PROPERTY_USAGE_DEFAULT, "TerraChunk"), "chunks_set", "chunks_get");

	ClassDB::bind_method(D_METHOD("generation_queue_add_to", "chunk"), &TerraWorld::generation_queue_add_to);
	ClassDB::bind_method(D_METHOD("generation_queue_get_index", "index"), &TerraWorld::generation_queue_get_index);
	ClassDB::bind_method(D_METHOD("generation_queue_remove_index", "index"), &TerraWorld::generation_queue_remove_index);
	ClassDB::bind_method(D_METHOD("generation_queue_get_size"), &TerraWorld::generation_queue_get_size);

	ClassDB::bind_method(D_METHOD("generation_add_to", "chunk"), &TerraWorld::generation_add_to);
	ClassDB::bind_method(D_METHOD("generation_get_index", "index"), &TerraWorld::generation_get_index);
	ClassDB::bind_method(D_METHOD("generation_remove_index", "index"), &TerraWorld::generation_remove_index);
	ClassDB::bind_method(D_METHOD("generation_get_size"), &TerraWorld::generation_get_size);

	ADD_SIGNAL(MethodInfo("generation_finished"));

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_generation_finished"));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk"), "_create_chunk", PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "z"), PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
	BIND_VMETHOD(MethodInfo("_prepare_chunk_for_generation", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk")));
#else
	GDVIRTUAL_BIND(_generation_finished);
	GDVIRTUAL_BIND(_create_chunk, "chunk", "x", "z", "chunk", "ret");
	GDVIRTUAL_BIND(_prepare_chunk_for_generation, "chunk");
	GDVIRTUAL_BIND(_generate_chunk, "chunk");
#endif

	ClassDB::bind_method(D_METHOD("chunk_get_or_create", "x", "z"), &TerraWorld::chunk_get_or_create);
	ClassDB::bind_method(D_METHOD("chunk_create", "x", "z"), &TerraWorld::chunk_create);
	ClassDB::bind_method(D_METHOD("chunk_setup", "chunk"), &TerraWorld::chunk_setup);

	ClassDB::bind_method(D_METHOD("_create_chunk", "x", "z", "chunk"), &TerraWorld::_create_chunk);
	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &TerraWorld::_generate_chunk);

	ClassDB::bind_method(D_METHOD("can_chunk_do_build_step"), &TerraWorld::can_chunk_do_build_step);
	ClassDB::bind_method(D_METHOD("is_position_walkable", "position"), &TerraWorld::is_position_walkable);
	ClassDB::bind_method(D_METHOD("on_chunk_mesh_generation_finished", "chunk"), &TerraWorld::on_chunk_mesh_generation_finished);

#if PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_add", "transform", "prop", "apply_voxel_scale"), &TerraWorld::prop_add, DEFVAL(true));
#endif

	//Lights
	ClassDB::bind_method(D_METHOD("light_add", "light"), &TerraWorld::light_add);
	ClassDB::bind_method(D_METHOD("light_get", "index"), &TerraWorld::light_get);
	ClassDB::bind_method(D_METHOD("light_remove", "index"), &TerraWorld::light_remove);
	ClassDB::bind_method(D_METHOD("light_get_count"), &TerraWorld::light_get_count);
	ClassDB::bind_method(D_METHOD("lights_clear"), &TerraWorld::lights_clear);

	ClassDB::bind_method(D_METHOD("lights_get"), &TerraWorld::lights_get);
	ClassDB::bind_method(D_METHOD("lights_set", "chunks"), &TerraWorld::lights_set);

	ClassDB::bind_method(D_METHOD("get_voxel_at_world_position", "world_position", "channel_index"), &TerraWorld::get_voxel_at_world_position);
	ClassDB::bind_method(D_METHOD("set_voxel_at_world_position", "world_position", "data", "channel_index", "rebuild"), &TerraWorld::set_voxel_at_world_position, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_chunk_at_world_position", "world_position"), &TerraWorld::get_chunk_at_world_position);
	ClassDB::bind_method(D_METHOD("get_or_create_chunk_at_world_position", "world_position"), &TerraWorld::get_or_create_chunk_at_world_position);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::INT, "ret"), "_get_channel_index_info", PropertyInfo(Variant::INT, "channel_type", PROPERTY_HINT_ENUM, BINDING_STRING_CHANNEL_TYPE_INFO)));
#else
	GDVIRTUAL_BIND(_get_channel_index_info, "channel_type", "ret");
#endif

	ClassDB::bind_method(D_METHOD("get_channel_index_info", "channel_type"), &TerraWorld::get_channel_index_info);
	ClassDB::bind_method(D_METHOD("_get_channel_index_info", "channel_type"), &TerraWorld::_get_channel_index_info);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_set_voxel_with_tool",
			PropertyInfo(Variant::BOOL, "mode_add"),
			PropertyInfo(Variant::VECTOR3, "hit_position"),
			PropertyInfo(Variant::VECTOR3, "hit_normal"),
			PropertyInfo(Variant::INT, "selected_voxel"),
			PropertyInfo(Variant::INT, "isolevel")));
#else
	GDVIRTUAL_BIND(_set_voxel_with_tool, "mode_add", "hit_position", "hit_normal", "selected_voxel", "isolevel");
#endif

	ClassDB::bind_method(D_METHOD("set_voxel_with_tool", "mode_add", "hit_position", "hit_normal", "selected_voxel", "isolevel"), &TerraWorld::set_voxel_with_tool);
	ClassDB::bind_method(D_METHOD("_set_voxel_with_tool", "mode_add", "hit_position", "hit_normal", "selected_voxel", "isolevel"), &TerraWorld::_set_voxel_with_tool);

	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_TYPE);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_ISOLEVEL);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_LIQUID_FLOW);

	BIND_CONSTANT(NOTIFICATION_ACTIVE_STATE_CHANGED);
}
