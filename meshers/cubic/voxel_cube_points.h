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

#ifndef SUB_TERRA_POINTS_H
#define SUB_TERRA_POINTS_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/reference.h"
#include "core/templates/vector.h"
#else
#include "core/reference.h"
#include "core/vector.h"
#endif

class TerraChunk;
class SubTerraFacePointsHelper;

class TerraCubePoints : public Reference {
	GDCLASS(TerraCubePoints, Reference);

public:
	enum Points {
		P000 = 0,
		P100 = 1,
		P010 = 2,
		P001 = 3,

		P110 = 4,
		P011 = 5,
		P101 = 6,
		P111 = 7,

		POINT_COUNT = 8,
	};

	enum TerraFaces {
		TERRA_FACE_FRONT = 0,
		TERRA_FACE_RIGHT = 1,
		TERRA_FACE_BACK = 2,
		TERRA_FACE_LEFT = 3,
		TERRA_FACE_TOP = 4,
		TERRA_FACE_BOTTOM = 5,
		TERRA_FACE_COUNT = 6,
	};

	enum TerraNeighbours {
		TERRA_NEIGHBOUR_NONE = 0,

		TERRA_NEIGHBOUR_LEFT = 1 << 0,
		TERRA_NEIGHBOUR_RIGHT = 1 << 1,
		TERRA_NEIGHBOUR_FRONT = 1 << 2,
		TERRA_NEIGHBOUR_BACK = 1 << 3,
		TERRA_NEIGHBOUR_TOP = 1 << 4,
		TERRA_NEIGHBOUR_BOTTOM = 1 << 5,

		TERRA_NEIGHBOUR_LEFT_FRONT = 1 << 6,
		TERRA_NEIGHBOUR_LEFT_BACK = 1 << 7,
		TERRA_NEIGHBOUR_RIGHT_FRONT = 1 << 8,
		TERRA_NEIGHBOUR_RIGHT_BACK = 1 << 9,

		TERRA_NEIGHBOUR_TOP_LEFT = 1 << 10,
		TERRA_NEIGHBOUR_TOP_RIGHT = 1 << 11,
		TERRA_NEIGHBOUR_TOP_FRONT = 1 << 12,
		TERRA_NEIGHBOUR_TOP_BACK = 1 << 13,

		TERRA_NEIGHBOUR_BOTTOM_LEFT = 1 << 14,
		TERRA_NEIGHBOUR_BOTTOM_RIGHT = 1 << 15,
		TERRA_NEIGHBOUR_BOTTOM_FRONT = 1 << 16,
		TERRA_NEIGHBOUR_BOTTOM_BACK = 1 << 17,

		TERRA_NEIGHBOUR_BOTTOM_LEFT_FRONT = 1 << 18,
		TERRA_NEIGHBOUR_BOTTOM_LEFT_BACK = 1 << 19,
		TERRA_NEIGHBOUR_BOTTOM_RIGHT_FRONT = 1 << 20,
		TERRA_NEIGHBOUR_BOTTOM_RIGHT_BACK = 1 << 21,

		TERRA_NEIGHBOUR_TOP_LEFT_FRONT = 1 << 22,
		TERRA_NEIGHBOUR_TOP_LEFT_BACK = 1 << 23,
		TERRA_NEIGHBOUR_TOP_RIGHT_FRONT = 1 << 24,
		TERRA_NEIGHBOUR_TOP_RIGHT_BACK = 1 << 25,

		TERRA_FULL_NEIGHBOURS_CROSS = TERRA_NEIGHBOUR_LEFT | TERRA_NEIGHBOUR_RIGHT | TERRA_NEIGHBOUR_BACK | TERRA_NEIGHBOUR_FRONT | TERRA_NEIGHBOUR_TOP | TERRA_NEIGHBOUR_BOTTOM, // | TERRA_NEIGHBOUR_LEFT_BACK | TERRA_NEIGHBOUR_LEFT_FRONT | TERRA_NEIGHBOUR_RIGHT_BACK | TERRA_NEIGHBOUR_RIGHT_FRONT;
		TERRA_FULL_SIDE_NEIGHBOURS = TERRA_NEIGHBOUR_LEFT | TERRA_NEIGHBOUR_RIGHT | TERRA_NEIGHBOUR_BACK | TERRA_NEIGHBOUR_FRONT,
		TERRA_FULL_SIDE_NEIGHBOURS_TOP = TERRA_NEIGHBOUR_LEFT | TERRA_NEIGHBOUR_RIGHT | TERRA_NEIGHBOUR_BACK | TERRA_NEIGHBOUR_FRONT | TERRA_NEIGHBOUR_TOP,
		TERRA_FULL_SIDE_NEIGHBOURS_DOWN = TERRA_NEIGHBOUR_LEFT | TERRA_NEIGHBOUR_RIGHT | TERRA_NEIGHBOUR_BACK | TERRA_NEIGHBOUR_FRONT | TERRA_NEIGHBOUR_BOTTOM,
	};

	int get_x();
	void set_x(int value);

	int get_y();
	void set_y(int value);

	int get_z();
	void set_z(int value);

	int get_size();
	void set_size(int value);

	int get_channel_index_type() const;
	void set_channel_index_type(const int value);

	int get_channel_index_isolevel() const;
	void set_channel_index_isolevel(const int value);

	void refresh_points();
	void recalculate_point(int point);
	void refresh_neighbours(Ref<TerraChunk> chunk);
	void setup(Ref<TerraChunk> chunk, int x, int y, int z, int size = 1);

	void reset();

	int get_point_index(int face, int index);
	Vector2 get_point_uv_direction(int face, int index);

	Vector3 get_points_for_face(int face, int index);
	bool is_face_visible(int face);
	bool is_sub_voxel_point(int x, int y, int z);
	void set_point(int point, int x, int y, int z);
	int get_point_id(int x, int y, int z);

	Vector3 get_point_for_face(int face, int pointIndex);
	Vector3 get_vertex_vector3_for_point(int face, int pointIndex);

	int get_point_type(int index);
	int get_point_fill(int index);
	int get_point_neighbours(int index);

	int get_point_ao(int index);
	int get_face_point_ao(int face, int index);
	Color get_face_point_ao_color(int face, int index);
	Color get_face_point_light_color(int face, int index);
	Color get_face_point_color_mixed(int face, int index);

	Vector3 get_face_light_direction(int face);

	Vector3 get_point(int index);
	Vector3 get_top_left_point(int face);
	Vector3 get_top_right_point(int face);
	Vector3 get_bottom_left_point(int face);
	Vector3 get_bottom_right_point(int face);

	uint8_t get_face_type(int face);

	bool has_points();
	int get_opposite_face(int face);

	TerraCubePoints();
	~TerraCubePoints();

protected:
	static void _bind_methods();

	static const unsigned int index_table[6][4];
	static const unsigned int visibility_check_table[6];
	static const int face_light_direction_table[6][3];
	static const float point_direction_table[8][3];
	static const unsigned int point_direction_neighbour_table[8][3];
	static const float uv_direction_table[8][4][2];

private:
	int _channel_index_type;
	int _channel_index_isolevel;

	Vector3 _points[POINT_COUNT];

	uint8_t _point_types[POINT_COUNT];
	uint8_t _point_fills[POINT_COUNT];
	uint8_t _point_aos[POINT_COUNT];
	Color _point_colors[POINT_COUNT];
	unsigned int _point_neighbours[POINT_COUNT];

	int _size;
	int _x;
	int _y;
	int _z;
};

VARIANT_ENUM_CAST(TerraCubePoints::Points);
VARIANT_ENUM_CAST(TerraCubePoints::TerraFaces);
VARIANT_ENUM_CAST(TerraCubePoints::TerraNeighbours);

#endif
