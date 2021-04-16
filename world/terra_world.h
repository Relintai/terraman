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

#ifndef TERRA_WORLD_H
#define TERRA_WORLD_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/templates/hash_map.h"
#include "core/config/engine.h"
#else
#include "core/hash_map.h"
#include "core/engine.h"
#endif

#include "../defines.h"

#include navigation_h

#include "../areas/terra_world_area.h"
#include "../level_generator/terraman_level_generator.h"
#include "../library/terraman_library.h"

#include "core/os/os.h"

#if PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

class TerraStructure;
class TerraChunk;
class PropData;

class TerraWorld : public Navigation {
	GDCLASS(TerraWorld, Navigation);

public:
	enum ChannelTypeInfo {
		CHANNEL_TYPE_INFO_TYPE = 0,
		CHANNEL_TYPE_INFO_ISOLEVEL,
		CHANNEL_TYPE_INFO_LIQUID_FLOW,
	};

	static const String BINDING_STRING_CHANNEL_TYPE_INFO;

public:
	bool get_editable() const;
	void set_editable(const bool value);

	int get_chunk_size_x() const;
	void set_chunk_size_x(const int value);

	int get_chunk_size_z() const;
	void set_chunk_size_z(const int value);

	int get_data_margin_start() const;
	void set_data_margin_start(const int value);

	int get_data_margin_end() const;
	void set_data_margin_end(const int value);

	int get_current_seed() const;
	void set_current_seed(const int value);

	bool get_use_threads() const;
	void set_use_threads(const bool value);

	float get_world_height() const;
	void set_world_height(const float value);

	int get_max_concurrent_generations() const;
	void set_max_concurrent_generations(const int value);

	int get_max_frame_chunk_build_steps() const;
	void set_max_frame_chunk_build_steps(const int value);

	Ref<TerramanLibrary> get_library();
	void set_library(const Ref<TerramanLibrary> &library);

	Ref<TerramanLevelGenerator> get_level_generator() const;
	void set_level_generator(const Ref<TerramanLevelGenerator> &level_generator);

	float get_voxel_scale() const;
	void set_voxel_scale(const float value);

	int get_chunk_spawn_range() const;
	void set_chunk_spawn_range(const int value);

	NodePath get_player_path() const;
	void set_player_path(const NodePath &player_path);

	Spatial *get_player() const;
	void set_player(Spatial *player);
	void set_player_bind(Node *player);

	//World Areas
	Ref<TerraWorldArea> world_area_get(const int index) const;
	void world_area_add(const Ref<TerraWorldArea> &area);
	void world_area_remove(const int index);
	void world_areas_clear();
	int world_area_get_count() const;

	//Terra Structures
	Ref<TerraStructure> voxel_structure_get(const int index) const;
	void voxel_structure_add(const Ref<TerraStructure> &structure);
	void voxel_structure_remove(const Ref<TerraStructure> &structure);
	void voxel_structure_remove_index(const int index);
	void voxel_structures_clear();
	int voxel_structure_get_count() const;
	void voxel_structure_add_at_position(Ref<TerraStructure> structure, const Vector3 &world_position);

	Vector<Variant> voxel_structures_get();
	void voxel_structures_set(const Vector<Variant> &structures);

	//Chunks
	void chunk_add(Ref<TerraChunk> chunk, const int x, const int z);
	bool chunk_has(const int x, const int z) const;
	Ref<TerraChunk> chunk_get(const int x, const int z);
	Ref<TerraChunk> chunk_remove(const int x, const int z);
	Ref<TerraChunk> chunk_remove_index(const int index);
	Ref<TerraChunk> chunk_get_index(const int index);

	int chunk_get_count() const;

	void chunks_clear();

	Ref<TerraChunk> chunk_get_or_create(const int x, const int z);
	Ref<TerraChunk> chunk_create(const int x, const int z);
	void chunk_setup(Ref<TerraChunk> chunk);

	void chunk_generate(Ref<TerraChunk> chunk);

	Vector<Variant> chunks_get();
	void chunks_set(const Vector<Variant> &chunks);

	bool can_chunk_do_build_step();
	bool is_position_walkable(const Vector3 &p_pos);

	void on_chunk_mesh_generation_finished(Ref<TerraChunk> p_chunk);

	void generation_queue_add_to(const Ref<TerraChunk> &chunk);
	Ref<TerraChunk> generation_queue_get_index(const int index);
	void generation_queue_remove_index(const int index);
	int generation_queue_get_size() const;

	void generation_add_to(const Ref<TerraChunk> &chunk);
	Ref<TerraChunk> generation_get_index(const int index);
	void generation_remove_index(const int index);
	int generation_get_size() const;

#if PROPS_PRESENT
	void prop_add(Transform tarnsform, const Ref<PropData> &prop, const bool apply_voxel_scael = true);
#endif

	//Lights
	void light_add(const Ref<TerraLight> &light);
	Ref<TerraLight> light_get(const int index);
	void light_remove(const int index);
	int light_get_count() const;
	void lights_clear();

	Vector<Variant> lights_get();
	void lights_set(const Vector<Variant> &chunks);

	//Helpers
	uint8_t get_voxel_at_world_position(const Vector3 &world_position, const int channel_index);
	void set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index, const bool rebuild = true);
	Ref<TerraChunk> get_chunk_at_world_position(const Vector3 &world_position);
	Ref<TerraChunk> get_or_create_chunk_at_world_position(const Vector3 &world_position);
	void set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel);

	int get_channel_index_info(const ChannelTypeInfo channel_type);

	TerraWorld();
	~TerraWorld();

protected:
	virtual void _generate_chunk(Ref<TerraChunk> chunk);
	virtual Ref<TerraChunk> _create_chunk(int x, int z, Ref<TerraChunk> p_chunk);
	virtual int _get_channel_index_info(const ChannelTypeInfo channel_type);
	virtual void _set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel);

	virtual void _notification(int p_what);
	static void _bind_methods();

public:
	struct IntPos {
		int x;
		int y;
		int z;

		IntPos() {
			x = 0;
			z = 0;
		}

		IntPos(int p_x, int p_z) {
			x = p_x;
			z = p_z;
		}

		IntPos(const Vector2 &p) {
			x = p.x;
			z = p.y;
		}
	};

	struct IntPosHasher {
		static _FORCE_INLINE_ uint32_t hash(const IntPos &v) {
			uint32_t hash = hash_djb2_one_32(v.x);
			hash = hash_djb2_one_32(v.x, hash);
			return hash_djb2_one_32(v.z, hash);
		}
	};

private:
	bool _editable;

	bool _is_priority_generation;

	int _chunk_size_x;
	int _chunk_size_z;
	int _current_seed;
	int _data_margin_start;
	int _data_margin_end;
	float _world_height;

	Ref<TerramanLibrary> _library;
	Ref<TerramanLevelGenerator> _level_generator;
	float _voxel_scale;
	int _chunk_spawn_range;

	HashMap<IntPos, Ref<TerraChunk>, IntPosHasher> _chunks;
	Vector<Ref<TerraChunk> > _chunks_vector;

	Vector<Ref<TerraWorldArea> > _world_areas;

	Vector<Ref<TerraStructure> > _voxel_structures;

	NodePath _player_path;
	Spatial *_player;

	bool _use_threads;
	int _max_concurrent_generations;
	Vector<Ref<TerraChunk> > _generation_queue;
	Vector<Ref<TerraChunk> > _generating;
	int _max_frame_chunk_build_steps;
	int _num_frame_chunk_build_steps;

	Vector<Ref<TerraLight> > _lights;
};

_FORCE_INLINE_ bool operator==(const TerraWorld::IntPos &a, const TerraWorld::IntPos &b) {
	return a.x == b.x && a.z == b.z;
}

VARIANT_ENUM_CAST(TerraWorld::ChannelTypeInfo);

#endif
