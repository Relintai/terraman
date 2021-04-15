#include "marching_cubes_tables.h"

//================================================================================
//
// The MarchingCubes Algorithm look-up tables
//
// These tables have been modified, in order to implement uv mapping.
// Also the transition data have been removed.
//
// Copyright 2009 by Eric Lengyel
//
// The following data originates from Eric Lengyel's MarchingCubes Algorithm.
// http://transvoxel.org/
//
// The data in this file may be freely used in implementations of the MarchingCubes
// Algorithm. If you do use this data, or any transformation of it, in your own
// projects, commercial or otherwise, please give credit by indicating in your
// source code that the data is part of the author's implementation of the
// MarchingCubes Algorithm and that it came from the web address given above.
// (Simply copying and pasting the two lines of the previous paragraph would be
// perfect.) If you distribute a commercial product with source code included,
// then the credit in the source code is required.
//
// If you distribute any kind of product that uses this data, a credit visible to
// the end-user would be appreciated, but it is not required. However, you may
// not claim that the entire implementation of the MarchingCubes Algorithm is your
// own if you use the data in this file or any transformation of it.
//
// The format of the data in this file is described in the dissertation "Terra-
// Based Terrain for Real-Time Virtual Simulations", available at the web page
// given above. References to sections and figures below pertain to that paper.
//
// The contents of this file are protected by copyright and may not be publicly
// reproduced without permission.
//
//================================================================================

namespace MarchingCubes {

// The regularCellClass table maps an 8-bit regular Marching Cubes case index to
// an equivalence class index. Even though there are 18 equivalence classes in our
// modified Marching Cubes algorithm, a couple of them use the same exact triangulations,
// just with different vertex locations. We combined those classes for this table so
// that the class index ranges from 0 to 15.

const unsigned char regularCellClass[256] = {
	//    0    1     2     3     4     5      6    7     8     9     10    11    12    13    14    15
	0x00, 0x01, 0x01, 0x03, 0x01, 0x03, 0x02, 0x04, 0x01, 0x02, 0x03, 0x04, 0x03, 0x04, 0x04, 0x03, // 0
	0x01, 0x03, 0x02, 0x04, 0x02, 0x04, 0x06, 0x0C, 0x02, 0x05, 0x05, 0x0B, 0x05, 0x0A, 0x07, 0x04, // 16
	0x01, 0x02, 0x03, 0x04, 0x02, 0x05, 0x05, 0x0A, 0x02, 0x06, 0x04, 0x0C, 0x05, 0x07, 0x0B, 0x04, // 32
	0x03, 0x04, 0x04, 0x03, 0x05, 0x0B, 0x07, 0x04, 0x05, 0x07, 0x0A, 0x04, 0x08, 0x0E, 0x0E, 0x03, // 48
	0x01, 0x02, 0x02, 0x05, 0x03, 0x04, 0x05, 0x0B, 0x02, 0x06, 0x05, 0x07, 0x04, 0x0C, 0x0A, 0x04, // 64
	0x03, 0x04, 0x05, 0x0A, 0x04, 0x03, 0x07, 0x04, 0x05, 0x07, 0x08, 0x0E, 0x0B, 0x04, 0x0E, 0x03, // 80
	0x02, 0x06, 0x05, 0x07, 0x05, 0x07, 0x08, 0x0E, 0x06, 0x09, 0x07, 0x0F, 0x07, 0x0F, 0x0E, 0x0D, // 96
	0x04, 0x0C, 0x0B, 0x04, 0x0A, 0x04, 0x0E, 0x03, 0x07, 0x0F, 0x0E, 0x0D, 0x0E, 0x0D, 0x02, 0x01, // 112
	0x01, 0x02, 0x02, 0x05, 0x02, 0x05, 0x06, 0x07, 0x03, 0x05, 0x04, 0x0A, 0x04, 0x0B, 0x0C, 0x04, // 128
	0x02, 0x05, 0x06, 0x07, 0x06, 0x07, 0x09, 0x0F, 0x05, 0x08, 0x07, 0x0E, 0x07, 0x0E, 0x0F, 0x0D, // 144
	0x03, 0x05, 0x04, 0x0B, 0x05, 0x08, 0x07, 0x0E, 0x04, 0x07, 0x03, 0x04, 0x0A, 0x0E, 0x04, 0x03, // 160
	0x04, 0x0A, 0x0C, 0x04, 0x07, 0x0E, 0x0F, 0x0D, 0x0B, 0x0E, 0x04, 0x03, 0x0E, 0x02, 0x0D, 0x01, // 176
	0x03, 0x05, 0x05, 0x08, 0x04, 0x0A, 0x07, 0x0E, 0x04, 0x07, 0x0B, 0x0E, 0x03, 0x04, 0x04, 0x03, // 192
	0x04, 0x0B, 0x07, 0x0E, 0x0C, 0x04, 0x0F, 0x0D, 0x0A, 0x0E, 0x0E, 0x02, 0x04, 0x03, 0x0D, 0x01, // 208
	0x04, 0x07, 0x0A, 0x0E, 0x0B, 0x0E, 0x0E, 0x02, 0x0C, 0x0F, 0x04, 0x0D, 0x04, 0x0D, 0x03, 0x01, // 224
	0x03, 0x04, 0x04, 0x03, 0x04, 0x03, 0x0D, 0x01, 0x04, 0x0D, 0x03, 0x01, 0x03, 0x01, 0x01, 0x00 // 240
};

// The regularCellData table holds the triangulation data for all 16 distinct classes to
// which a case can be mapped by the regularCellClass table.

const RegularCellData regularCellData[16] = {
	{ 0x00, {} },
	{ 0x31, { 0, 1, 2 } },
	{ 0x62, { 0, 1, 2, 3, 4, 5 } },
	{ 0x62, { 0, 1, 2, 4, 5, 3 } }, //{ 0x42, { 0, 1, 2, 0, 2, 3 } }
	{ 0x93, { 0, 1, 4, 5, 3, 6, 7, 2, 8 } }, //{ 0x53, { 0, 1, 4,  1, 3, 4,  1, 2, 3 } }
	{ 0x73, { 0, 1, 2, 0, 2, 3, 4, 5, 6 } },
	{ 0x93, { 0, 1, 2, 3, 4, 5, 6, 7, 8 } },
	{ 0xC4, { 0, 1, 4, 8, 3, 9, 10, 2, 11, 5, 6, 7 } }, //7 { 0x84, { 0, 1, 4,  1, 3, 4,  1, 2, 3,  5, 6, 7 } },
	{ 0x84, { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7 } },
	{ 0xC4, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } },
	{ 0xC4, { 0, 4, 5, 6, 7, 8, 9, 10, 11, 1, 2, 3 } }, //A { 0x64, { 0, 4, 5,  0, 1, 4,  1, 3, 4,  1, 2, 3 } },
	{ 0xC4, { 0, 5, 4, 6, 8, 7, 9, 11, 10, 1, 3, 2 } }, //B { 0x64, { 0, 5, 4,  0, 4, 1,  1, 4, 3,  1, 3, 2 } },
	{ 0xC4, { 0, 4, 5, 6, 7, 8, 9, 10, 11, 1, 2, 3 } }, //C { 0x64, { 0, 4, 5,  0, 3, 4,  0, 1, 3,  1, 2, 3 } },
	{ 0xC4, { 0, 1, 2, 6, 7, 3, 8, 9, 4, 10, 11, 5 } }, //D { 0x64, { 0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 5 } },
	{ 0xF5, { 0, 1, 2, 7, 8, 3, 9, 10, 4, 11, 12, 5, 13, 14, 6 } }, //E { 0x75, { 0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 5,  0, 5, 6 } },
	{ 0xF5, { 0, 4, 5, 9, 3, 10, 11, 1, 12, 13, 2, 14, 6, 7, 8 } } //F { 0x95, { 0, 4, 5,  0, 3, 4,  0, 1, 3,  1, 2, 3,  6, 7, 8 } }
};

// The regularVertexData table gives the vertex locations for every one of the 256 possible
// cases in the modified Marching Cubes algorithm. Each 16-bit value also provides information
// about whether a vertex can be reused from a neighboring cell. See Section 3.3 for details.
// The low byte contains the indexes for the two endpoints of the edge on which the vertex lies,
// as numbered in Figure 3.7. The high byte contains the vertex reuse data shown in Figure 3.8.

const unsigned short regularVertexData[256][15] = {
	{},
	{ 0x6201, 0x5102, 0x3304 },
	{ 0x6201, 0x2315, 0x4113 },
	{ 0x5102, 0x3304, 0x2315, 0x4113, 0x5102, 0x2315 },
	{ 0x5102, 0x4223, 0x1326 },
	{ 0x3304, 0x6201, 0x4223, 0x1326, 0x3304, 0x4223 },
	{ 0x6201, 0x2315, 0x4113, 0x5102, 0x4223, 0x1326 },
	{ 0x4223, 0x1326, 0x3304, 0x2315, 0x4113, 0x1326, 0x4113, 0x1326, 0x2315 },
	{ 0x4113, 0x8337, 0x4223 },
	{ 0x6201, 0x5102, 0x3304, 0x4223, 0x4113, 0x8337 },
	{ 0x6201, 0x2315, 0x8337, 0x4223, 0x6201, 0x8337 },
	{ 0x5102, 0x3304, 0x2315, 0x8337, 0x4223, 0x3304, 0x4223, 0x3304, 0x8337 },
	{ 0x5102, 0x4113, 0x8337, 0x1326, 0x5102, 0x8337 },
	{ 0x4113, 0x8337, 0x1326, 0x3304, 0x6201, 0x8337, 0x6201, 0x8337, 0x3304 },
	{ 0x6201, 0x2315, 0x8337, 0x1326, 0x5102, 0x2315, 0x5102, 0x2315, 0x1326 },
	{ 0x3304, 0x2315, 0x8337, 0x1326, 0x3304, 0x8337 },
	//16  3 5 15
	{ 0x3304, 0x1146, 0x2245 },
	{ 0x6201, 0x5102, 0x1146, 0x2245, 0x6201, 0x1146 },
	{ 0x6201, 0x2315, 0x4113, 0x3304, 0x1146, 0x2245 },
	{ 0x2315, 0x4113, 0x5102, 0x1146, 0x2245, 0x4113, 0x2245, 0x4113, 0x1146 },
	{ 0x5102, 0x4223, 0x1326, 0x3304, 0x1146, 0x2245 },
	{ 0x1146, 0x2245, 0x6201, 0x4223, 0x1326, 0x2245, 0x1326, 0x2245, 0x4223 },
	{ 0x3304, 0x1146, 0x2245, 0x6201, 0x2315, 0x4113, 0x5102, 0x4223, 0x1326 },
	{ 0x4223, 0x1326, 0x1146, 0x2245, 0x2315, 0x4113, 0x4223, 0x2245, 0x2315, 0x4223, 0x1326, 0x2245 },
	{ 0x4223, 0x4113, 0x8337, 0x3304, 0x1146, 0x2245 },
	{ 0x6201, 0x5102, 0x1146, 0x2245, 0x4223, 0x4113, 0x8337 },
	{ 0x4223, 0x6201, 0x2315, 0x8337, 0x3304, 0x1146, 0x2245 },
	{ 0x4223, 0x8337, 0x2315, 0x2245, 0x1146, 0x5102, 0x4223, 0x8337, 0x1146, 0x8337, 0x2245, 0x1146 },
	{ 0x5102, 0x4113, 0x8337, 0x1326, 0x3304, 0x1146, 0x2245 },
	{ 0x4113, 0x8337, 0x1326, 0x1146, 0x2245, 0x6201, 0x4113, 0x8337, 0x2245, 0x8337, 0x1146, 0x2245 },
	{ 0x6201, 0x2315, 0x8337, 0x1326, 0x5102, 0x3304, 0x1146, 0x2245, 0x2315, 0x5102, 0x2315, 0x1326 },
	{ 0x2245, 0x2315, 0x8337, 0x1326, 0x1146, 0x2315, 0x1146, 0x2315, 0x1326 },
	//32  3 10 15
	{ 0x2315, 0x2245, 0x8157 },
	{ 0x6201, 0x5102, 0x3304, 0x2315, 0x2245, 0x8157 },
	{ 0x4113, 0x6201, 0x2245, 0x8157, 0x4113, 0x2245 },
	{ 0x2245, 0x8157, 0x4113, 0x5102, 0x3304, 0x8157, 0x3304, 0x8157, 0x5102 },
	{ 0x5102, 0x4223, 0x1326, 0x2315, 0x2245, 0x8157 },
	{ 0x6201, 0x4223, 0x1326, 0x3304, 0x2315, 0x2245, 0x8157 },
	{ 0x6201, 0x2245, 0x8157, 0x4113, 0x5102, 0x4223, 0x1326 },
	{ 0x4223, 0x1326, 0x3304, 0x2245, 0x8157, 0x4113, 0x4223, 0x1326, 0x8157, 0x1326, 0x2245, 0x8157 },
	{ 0x4223, 0x4113, 0x8337, 0x2315, 0x2245, 0x8157 },
	{ 0x6201, 0x5102, 0x3304, 0x4223, 0x4113, 0x8337, 0x2315, 0x2245, 0x8157 },
	{ 0x8337, 0x4223, 0x6201, 0x2245, 0x8157, 0x4223, 0x8157, 0x4223, 0x2245 },
	{ 0x5102, 0x3304, 0x2245, 0x8157, 0x8337, 0x4223, 0x5102, 0x8157, 0x8337, 0x5102, 0x3304, 0x8157 },
	{ 0x5102, 0x4113, 0x8337, 0x1326, 0x2315, 0x2245, 0x8157 },
	{ 0x4113, 0x8337, 0x1326, 0x3304, 0x6201, 0x2315, 0x2245, 0x8157, 0x8337, 0x6201, 0x8337, 0x3304 },
	{ 0x5102, 0x1326, 0x8337, 0x8157, 0x2245, 0x6201, 0x5102, 0x1326, 0x2245, 0x1326, 0x8157, 0x2245 },
	{ 0x8157, 0x8337, 0x1326, 0x3304, 0x2245, 0x8337, 0x2245, 0x8337, 0x3304 },
	//48 1 2 7 11
	{ 0x2315, 0x3304, 0x1146, 0x8157, 0x2315, 0x1146 },
	{ 0x6201, 0x5102, 0x1146, 0x8157, 0x2315, 0x5102, 0x2315, 0x5102, 0x8157 },
	{ 0x3304, 0x1146, 0x8157, 0x4113, 0x6201, 0x1146, 0x6201, 0x1146, 0x4113 },
	{ 0x4113, 0x5102, 0x1146, 0x8157, 0x4113, 0x1146 },
	{ 0x2315, 0x3304, 0x1146, 0x8157, 0x5102, 0x4223, 0x1326 },
	{ 0x1326, 0x4223, 0x6201, 0x2315, 0x8157, 0x1146, 0x1326, 0x4223, 0x8157, 0x4223, 0x2315, 0x8157 },
	{ 0x3304, 0x1146, 0x8157, 0x4113, 0x6201, 0x5102, 0x4223, 0x1326, 0x1146, 0x6201, 0x1146, 0x4113 },
	{ 0x1326, 0x1146, 0x8157, 0x4113, 0x4223, 0x1146, 0x4223, 0x1146, 0x4113 },
	{ 0x2315, 0x3304, 0x1146, 0x8157, 0x4223, 0x4113, 0x8337 },
	{ 0x6201, 0x5102, 0x1146, 0x8157, 0x2315, 0x4223, 0x4113, 0x8337, 0x5102, 0x2315, 0x5102, 0x8157 },
	{ 0x3304, 0x1146, 0x8157, 0x8337, 0x4223, 0x6201, 0x3304, 0x1146, 0x4223, 0x1146, 0x8337, 0x4223 },
	{ 0x4223, 0x5102, 0x1146, 0x8157, 0x8337, 0x5102, 0x8337, 0x5102, 0x8157 },
	{ 0x2315, 0x3304, 0x1146, 0x8157, 0x5102, 0x4113, 0x8337, 0x1326 },
	{ 0x6201, 0x4113, 0x8337, 0x1326, 0x1146, 0x8157, 0x2315, 0x6201, 0x8337, 0x6201, 0x1326, 0x6201, 0x1146, 0x6201, 0x8157 },
	{ 0x6201, 0x3304, 0x1146, 0x8157, 0x8337, 0x1326, 0x5102, 0x6201, 0x1146, 0x6201, 0x8157, 0x6201, 0x8337, 0x6201, 0x1326 },
	{ 0x1326, 0x1146, 0x8157, 0x8337, 0x1326, 0x8157 },
	//64  5 12 15
	{ 0x1326, 0x8267, 0x1146 },
	{ 0x6201, 0x5102, 0x3304, 0x1326, 0x8267, 0x1146 },
	{ 0x6201, 0x2315, 0x4113, 0x1326, 0x8267, 0x1146 },
	{ 0x5102, 0x3304, 0x2315, 0x4113, 0x1326, 0x8267, 0x1146 },
	{ 0x5102, 0x4223, 0x8267, 0x1146, 0x5102, 0x8267 },
	{ 0x3304, 0x6201, 0x4223, 0x8267, 0x1146, 0x6201, 0x1146, 0x6201, 0x8267 },
	{ 0x5102, 0x4223, 0x8267, 0x1146, 0x6201, 0x2315, 0x4113 },
	{ 0x1146, 0x8267, 0x4223, 0x4113, 0x2315, 0x3304, 0x1146, 0x8267, 0x2315, 0x8267, 0x4113, 0x2315 },
	{ 0x4113, 0x8337, 0x4223, 0x1326, 0x8267, 0x1146 },
	{ 0x6201, 0x5102, 0x3304, 0x4223, 0x4113, 0x8337, 0x1326, 0x8267, 0x1146 },
	{ 0x6201, 0x2315, 0x8337, 0x4223, 0x1326, 0x8267, 0x1146 },
	{ 0x5102, 0x3304, 0x2315, 0x8337, 0x4223, 0x1326, 0x8267, 0x1146, 0x3304, 0x4223, 0x3304, 0x8337 },
	{ 0x8267, 0x1146, 0x5102, 0x4113, 0x8337, 0x1146, 0x8337, 0x1146, 0x4113 },
	{ 0x6201, 0x4113, 0x8337, 0x8267, 0x1146, 0x3304, 0x6201, 0x8267, 0x1146, 0x6201, 0x4113, 0x8267 },
	{ 0x6201, 0x2315, 0x8337, 0x8267, 0x1146, 0x5102, 0x6201, 0x2315, 0x1146, 0x2315, 0x8267, 0x1146 },
	{ 0x1146, 0x3304, 0x2315, 0x8337, 0x8267, 0x3304, 0x8267, 0x3304, 0x8337 },
	//80  1 4 7 13
	{ 0x3304, 0x1326, 0x8267, 0x2245, 0x3304, 0x8267 },
	{ 0x1326, 0x8267, 0x2245, 0x6201, 0x5102, 0x8267, 0x5102, 0x8267, 0x6201 },
	{ 0x3304, 0x1326, 0x8267, 0x2245, 0x6201, 0x2315, 0x4113 },
	{ 0x1326, 0x8267, 0x2245, 0x2315, 0x4113, 0x5102, 0x1326, 0x8267, 0x4113, 0x8267, 0x2315, 0x4113 },
	{ 0x5102, 0x4223, 0x8267, 0x2245, 0x3304, 0x4223, 0x3304, 0x4223, 0x2245 },
	{ 0x6201, 0x4223, 0x8267, 0x2245, 0x6201, 0x8267 },
	{ 0x5102, 0x4223, 0x8267, 0x2245, 0x3304, 0x6201, 0x2315, 0x4113, 0x4223, 0x3304, 0x4223, 0x2245 },
	{ 0x4113, 0x4223, 0x8267, 0x2245, 0x2315, 0x4223, 0x2315, 0x4223, 0x2245 },
	{ 0x3304, 0x1326, 0x8267, 0x2245, 0x4223, 0x4113, 0x8337 },
	{ 0x1326, 0x8267, 0x2245, 0x6201, 0x5102, 0x4223, 0x4113, 0x8337, 0x8267, 0x5102, 0x8267, 0x6201 },
	{ 0x3304, 0x1326, 0x8267, 0x2245, 0x4223, 0x6201, 0x2315, 0x8337 },
	{ 0x5102, 0x1326, 0x8267, 0x2245, 0x2315, 0x8337, 0x4223, 0x5102, 0x8267, 0x5102, 0x2245, 0x5102, 0x2315, 0x5102, 0x8337 },
	{ 0x3304, 0x2245, 0x8267, 0x8337, 0x4113, 0x5102, 0x3304, 0x2245, 0x4113, 0x2245, 0x8337, 0x4113 },
	{ 0x8337, 0x8267, 0x2245, 0x6201, 0x4113, 0x8267, 0x4113, 0x8267, 0x6201 },
	{ 0x5102, 0x6201, 0x2315, 0x8337, 0x8267, 0x2245, 0x3304, 0x5102, 0x2315, 0x5102, 0x8337, 0x5102, 0x8267, 0x5102, 0x2245 },
	{ 0x2315, 0x8337, 0x8267, 0x2245, 0x2315, 0x8267 },
	//96  -
	{ 0x2315, 0x2245, 0x8157, 0x1326, 0x8267, 0x1146 },
	{ 0x6201, 0x5102, 0x3304, 0x2315, 0x2245, 0x8157, 0x1326, 0x8267, 0x1146 },
	{ 0x6201, 0x2245, 0x8157, 0x4113, 0x1326, 0x8267, 0x1146 },
	{ 0x2245, 0x8157, 0x4113, 0x5102, 0x3304, 0x1326, 0x8267, 0x1146, 0x8157, 0x3304, 0x8157, 0x5102 },
	{ 0x4223, 0x8267, 0x1146, 0x5102, 0x2315, 0x2245, 0x8157 },
	{ 0x3304, 0x6201, 0x4223, 0x8267, 0x1146, 0x2315, 0x2245, 0x8157, 0x6201, 0x1146, 0x6201, 0x8267 },
	{ 0x4223, 0x8267, 0x1146, 0x5102, 0x6201, 0x2245, 0x8157, 0x4113 },
	{ 0x3304, 0x2245, 0x8157, 0x4113, 0x4223, 0x8267, 0x1146, 0x3304, 0x8157, 0x3304, 0x4113, 0x3304, 0x4223, 0x3304, 0x8267 },
	{ 0x4223, 0x4113, 0x8337, 0x2315, 0x2245, 0x8157, 0x1326, 0x8267, 0x1146 },
	{ 0x6201, 0x5102, 0x3304, 0x4223, 0x4113, 0x8337, 0x2315, 0x2245, 0x8157, 0x1326, 0x8267, 0x1146 },
	{ 0x8337, 0x4223, 0x6201, 0x2245, 0x8157, 0x1326, 0x8267, 0x1146, 0x4223, 0x8157, 0x4223, 0x2245 },
	{ 0x4223, 0x5102, 0x3304, 0x2245, 0x8157, 0x8337, 0x1326, 0x8267, 0x1146, 0x4223, 0x8157, 0x4223, 0x2245, 0x5102, 0x2245 },
	{ 0x8267, 0x1146, 0x5102, 0x4113, 0x8337, 0x2315, 0x2245, 0x8157, 0x1146, 0x8337, 0x1146, 0x4113 },
	{ 0x6201, 0x4113, 0x8337, 0x8267, 0x1146, 0x3304, 0x2315, 0x2245, 0x8157, 0x6201, 0x1146, 0x6201, 0x8267, 0x4113, 0x8267 },
	{ 0x8337, 0x8267, 0x1146, 0x5102, 0x6201, 0x2245, 0x8157, 0x8337, 0x1146, 0x8337, 0x5102, 0x8337, 0x6201, 0x8337, 0x2245 },
	{ 0x3304, 0x2245, 0x8157, 0x8337, 0x8267, 0x1146, 0x3304, 0x8157, 0x3304, 0x8337, 0x3304, 0x8267 },
	//112  0 3 5
	{ 0x8157, 0x2315, 0x3304, 0x1326, 0x8267, 0x2315, 0x8267, 0x2315, 0x1326 },
	{ 0x8267, 0x8157, 0x2315, 0x6201, 0x5102, 0x1326, 0x8267, 0x6201, 0x5102, 0x8267, 0x8157, 0x6201 },
	{ 0x8267, 0x1326, 0x3304, 0x6201, 0x4113, 0x8157, 0x8267, 0x1326, 0x4113, 0x1326, 0x6201, 0x4113 },
	{ 0x8267, 0x8157, 0x4113, 0x5102, 0x1326, 0x8157, 0x1326, 0x8157, 0x5102 },
	{ 0x5102, 0x4223, 0x8267, 0x8157, 0x2315, 0x3304, 0x5102, 0x4223, 0x2315, 0x4223, 0x8157, 0x2315 },
	{ 0x2315, 0x6201, 0x4223, 0x8267, 0x8157, 0x6201, 0x8157, 0x6201, 0x8267 },
	{ 0x3304, 0x5102, 0x4223, 0x8267, 0x8157, 0x4113, 0x6201, 0x3304, 0x4223, 0x3304, 0x8267, 0x3304, 0x8157, 0x3304, 0x4113 },
	{ 0x4113, 0x4223, 0x8267, 0x8157, 0x4113, 0x8267 },
	{ 0x8157, 0x2315, 0x3304, 0x1326, 0x8267, 0x4223, 0x4113, 0x8337, 0x2315, 0x8267, 0x2315, 0x1326 },
	{ 0x8157, 0x2315, 0x6201, 0x5102, 0x1326, 0x8267, 0x4223, 0x4113, 0x8337, 0x8157, 0x1326, 0x8157, 0x5102, 0x2315, 0x5102 },
	{ 0x8157, 0x8337, 0x4223, 0x6201, 0x3304, 0x1326, 0x8267, 0x8157, 0x4223, 0x8157, 0x6201, 0x8157, 0x3304, 0x8157, 0x1326 },
	{ 0x5102, 0x1326, 0x8267, 0x8157, 0x8337, 0x4223, 0x5102, 0x8267, 0x5102, 0x8157, 0x5102, 0x8337 },
	{ 0x8267, 0x8157, 0x2315, 0x3304, 0x5102, 0x4113, 0x8337, 0x8267, 0x2315, 0x8267, 0x3304, 0x8267, 0x5102, 0x8267, 0x4113 },
	{ 0x6201, 0x4113, 0x8337, 0x8267, 0x8157, 0x2315, 0x6201, 0x8337, 0x6201, 0x8267, 0x6201, 0x8157 },
	{ 0x6201, 0x3304, 0x5102, 0x8337, 0x8267, 0x8157 },
	{ 0x8337, 0x8267, 0x8157 },
	//128  10 12 15
	{ 0x8337, 0x8157, 0x8267 },
	{ 0x6201, 0x5102, 0x3304, 0x8337, 0x8157, 0x8267 },
	{ 0x6201, 0x2315, 0x4113, 0x8337, 0x8157, 0x8267 },
	{ 0x5102, 0x3304, 0x2315, 0x4113, 0x8337, 0x8157, 0x8267 },
	{ 0x5102, 0x4223, 0x1326, 0x8337, 0x8157, 0x8267 },
	{ 0x6201, 0x4223, 0x1326, 0x3304, 0x8337, 0x8157, 0x8267 },
	{ 0x6201, 0x2315, 0x4113, 0x5102, 0x4223, 0x1326, 0x8337, 0x8157, 0x8267 },
	{ 0x4223, 0x1326, 0x3304, 0x2315, 0x4113, 0x8337, 0x8157, 0x8267, 0x1326, 0x4113, 0x1326, 0x2315 },
	{ 0x4113, 0x8157, 0x8267, 0x4223, 0x4113, 0x8267 },
	{ 0x4223, 0x4113, 0x8157, 0x8267, 0x6201, 0x5102, 0x3304 },
	{ 0x8157, 0x8267, 0x4223, 0x6201, 0x2315, 0x8267, 0x2315, 0x8267, 0x6201 },
	{ 0x3304, 0x2315, 0x8157, 0x8267, 0x4223, 0x5102, 0x3304, 0x2315, 0x4223, 0x2315, 0x8267, 0x4223 },
	{ 0x1326, 0x5102, 0x4113, 0x8157, 0x8267, 0x5102, 0x8267, 0x5102, 0x8157 },
	{ 0x8157, 0x4113, 0x6201, 0x3304, 0x1326, 0x8267, 0x8157, 0x4113, 0x1326, 0x4113, 0x3304, 0x1326 },
	{ 0x1326, 0x5102, 0x6201, 0x2315, 0x8157, 0x8267, 0x1326, 0x2315, 0x8157, 0x1326, 0x5102, 0x2315 },
	{ 0x8267, 0x1326, 0x3304, 0x2315, 0x8157, 0x1326, 0x8157, 0x1326, 0x2315 },
	//144  -
	{ 0x3304, 0x1146, 0x2245, 0x8337, 0x8157, 0x8267 },
	{ 0x6201, 0x5102, 0x1146, 0x2245, 0x8337, 0x8157, 0x8267 },
	{ 0x6201, 0x2315, 0x4113, 0x3304, 0x1146, 0x2245, 0x8337, 0x8157, 0x8267 },
	{ 0x2315, 0x4113, 0x5102, 0x1146, 0x2245, 0x8337, 0x8157, 0x8267, 0x4113, 0x2245, 0x4113, 0x1146 },
	{ 0x5102, 0x4223, 0x1326, 0x3304, 0x1146, 0x2245, 0x8337, 0x8157, 0x8267 },
	{ 0x1146, 0x2245, 0x6201, 0x4223, 0x1326, 0x8337, 0x8157, 0x8267, 0x2245, 0x1326, 0x2245, 0x4223 },
	{ 0x6201, 0x2315, 0x4113, 0x5102, 0x4223, 0x1326, 0x3304, 0x1146, 0x2245, 0x8337, 0x8157, 0x8267 },
	{ 0x4113, 0x4223, 0x1326, 0x1146, 0x2245, 0x2315, 0x8337, 0x8157, 0x8267, 0x4113, 0x2245, 0x4113, 0x1146, 0x4223, 0x1146 },
	{ 0x4223, 0x4113, 0x8157, 0x8267, 0x3304, 0x1146, 0x2245 },
	{ 0x6201, 0x5102, 0x1146, 0x2245, 0x4223, 0x4113, 0x8157, 0x8267 },
	{ 0x8157, 0x8267, 0x4223, 0x6201, 0x2315, 0x3304, 0x1146, 0x2245, 0x8267, 0x2315, 0x8267, 0x6201 },
	{ 0x2315, 0x8157, 0x8267, 0x4223, 0x5102, 0x1146, 0x2245, 0x2315, 0x8267, 0x2315, 0x4223, 0x2315, 0x5102, 0x2315, 0x1146 },
	{ 0x1326, 0x5102, 0x4113, 0x8157, 0x8267, 0x3304, 0x1146, 0x2245, 0x5102, 0x8267, 0x5102, 0x8157 },
	{ 0x1326, 0x1146, 0x2245, 0x6201, 0x4113, 0x8157, 0x8267, 0x1326, 0x2245, 0x1326, 0x6201, 0x1326, 0x4113, 0x1326, 0x8157 },
	{ 0x5102, 0x6201, 0x2315, 0x8157, 0x8267, 0x1326, 0x3304, 0x1146, 0x2245, 0x5102, 0x8267, 0x5102, 0x8157, 0x6201, 0x8157 },
	{ 0x1326, 0x1146, 0x2245, 0x2315, 0x8157, 0x8267, 0x1326, 0x2245, 0x1326, 0x2315, 0x1326, 0x8157 },
	//160  2 8 11 14
	{ 0x2315, 0x2245, 0x8267, 0x8337, 0x2315, 0x8267 },
	{ 0x2315, 0x2245, 0x8267, 0x8337, 0x6201, 0x5102, 0x3304 },
	{ 0x4113, 0x6201, 0x2245, 0x8267, 0x8337, 0x6201, 0x8337, 0x6201, 0x8267 },
	{ 0x5102, 0x4113, 0x8337, 0x8267, 0x2245, 0x3304, 0x5102, 0x4113, 0x2245, 0x4113, 0x8267, 0x2245 },
	{ 0x2315, 0x2245, 0x8267, 0x8337, 0x5102, 0x4223, 0x1326 },
	{ 0x6201, 0x4223, 0x1326, 0x3304, 0x8337, 0x2315, 0x2245, 0x8267 },
	{ 0x4113, 0x6201, 0x2245, 0x8267, 0x8337, 0x5102, 0x4223, 0x1326, 0x6201, 0x8337, 0x6201, 0x8267 },
	{ 0x4113, 0x4223, 0x1326, 0x3304, 0x2245, 0x8267, 0x8337, 0x4113, 0x1326, 0x4113, 0x3304, 0x4113, 0x2245, 0x4113, 0x8267 },
	{ 0x2315, 0x2245, 0x8267, 0x4223, 0x4113, 0x2245, 0x4113, 0x2245, 0x4223 },
	{ 0x2315, 0x2245, 0x8267, 0x4223, 0x4113, 0x6201, 0x5102, 0x3304, 0x2245, 0x4113, 0x2245, 0x4223 },
	{ 0x6201, 0x2245, 0x8267, 0x4223, 0x6201, 0x8267 },
	{ 0x3304, 0x2245, 0x8267, 0x4223, 0x5102, 0x2245, 0x5102, 0x2245, 0x4223 },
	{ 0x5102, 0x4113, 0x2315, 0x2245, 0x8267, 0x1326, 0x5102, 0x4113, 0x8267, 0x4113, 0x2245, 0x8267 },
	{ 0x4113, 0x2315, 0x2245, 0x8267, 0x1326, 0x3304, 0x6201, 0x4113, 0x2245, 0x4113, 0x8267, 0x4113, 0x1326, 0x4113, 0x3304 },
	{ 0x5102, 0x6201, 0x2245, 0x8267, 0x1326, 0x6201, 0x1326, 0x6201, 0x8267 },
	{ 0x3304, 0x2245, 0x8267, 0x1326, 0x3304, 0x8267 },
	//176  0 3 10
	{ 0x8267, 0x8337, 0x2315, 0x3304, 0x1146, 0x8337, 0x1146, 0x8337, 0x3304 },
	{ 0x5102, 0x1146, 0x8267, 0x8337, 0x2315, 0x6201, 0x5102, 0x1146, 0x2315, 0x1146, 0x8337, 0x2315 },
	{ 0x3304, 0x1146, 0x8267, 0x8337, 0x4113, 0x6201, 0x3304, 0x8337, 0x4113, 0x3304, 0x1146, 0x8337 },
	{ 0x8337, 0x4113, 0x5102, 0x1146, 0x8267, 0x4113, 0x8267, 0x4113, 0x1146 },
	{ 0x8267, 0x8337, 0x2315, 0x3304, 0x1146, 0x5102, 0x4223, 0x1326, 0x8337, 0x1146, 0x8337, 0x3304 },
	{ 0x1146, 0x8267, 0x8337, 0x2315, 0x6201, 0x4223, 0x1326, 0x1146, 0x8337, 0x1146, 0x2315, 0x1146, 0x6201, 0x1146, 0x4223 },
	{ 0x8267, 0x8337, 0x4113, 0x6201, 0x3304, 0x1146, 0x5102, 0x4223, 0x1326, 0x8267, 0x3304, 0x8267, 0x6201, 0x8337, 0x6201 },
	{ 0x4113, 0x4223, 0x1326, 0x1146, 0x8267, 0x8337, 0x4113, 0x1326, 0x4113, 0x1146, 0x4113, 0x8267 },
	{ 0x3304, 0x2315, 0x4113, 0x4223, 0x8267, 0x1146, 0x3304, 0x2315, 0x8267, 0x2315, 0x4223, 0x8267 },
	{ 0x2315, 0x6201, 0x5102, 0x1146, 0x8267, 0x4223, 0x4113, 0x2315, 0x5102, 0x2315, 0x1146, 0x2315, 0x8267, 0x2315, 0x4223 },
	{ 0x1146, 0x8267, 0x4223, 0x6201, 0x3304, 0x8267, 0x3304, 0x8267, 0x6201 },
	{ 0x5102, 0x1146, 0x8267, 0x4223, 0x5102, 0x8267 },
	{ 0x8267, 0x1326, 0x5102, 0x4113, 0x2315, 0x3304, 0x1146, 0x8267, 0x5102, 0x8267, 0x4113, 0x8267, 0x2315, 0x8267, 0x3304 },
	{ 0x6201, 0x4113, 0x2315, 0x1326, 0x1146, 0x8267 },
	{ 0x6201, 0x3304, 0x1146, 0x8267, 0x1326, 0x5102, 0x6201, 0x1146, 0x6201, 0x8267, 0x6201, 0x1326 },
	{ 0x1326, 0x1146, 0x8267 },
	//192  4 8 13 14
	{ 0x1326, 0x8337, 0x8157, 0x1146, 0x1326, 0x8157 },
	{ 0x8337, 0x8157, 0x1146, 0x1326, 0x6201, 0x5102, 0x3304 },
	{ 0x8337, 0x8157, 0x1146, 0x1326, 0x6201, 0x2315, 0x4113 },
	{ 0x4113, 0x5102, 0x3304, 0x2315, 0x1326, 0x8337, 0x8157, 0x1146 },
	{ 0x8337, 0x8157, 0x1146, 0x5102, 0x4223, 0x8157, 0x4223, 0x8157, 0x5102 },
	{ 0x6201, 0x4223, 0x8337, 0x8157, 0x1146, 0x3304, 0x6201, 0x4223, 0x1146, 0x4223, 0x8157, 0x1146 },
	{ 0x8337, 0x8157, 0x1146, 0x5102, 0x4223, 0x6201, 0x2315, 0x4113, 0x8157, 0x4223, 0x8157, 0x5102 },
	{ 0x4223, 0x8337, 0x8157, 0x1146, 0x3304, 0x2315, 0x4113, 0x4223, 0x8157, 0x4223, 0x1146, 0x4223, 0x3304, 0x4223, 0x2315 },
	{ 0x4223, 0x4113, 0x8157, 0x1146, 0x1326, 0x4113, 0x1326, 0x4113, 0x1146 },
	{ 0x4223, 0x4113, 0x8157, 0x1146, 0x1326, 0x6201, 0x5102, 0x3304, 0x4113, 0x1326, 0x4113, 0x1146 },
	{ 0x1146, 0x8157, 0x2315, 0x6201, 0x4223, 0x1326, 0x1146, 0x8157, 0x4223, 0x8157, 0x6201, 0x4223 },
	{ 0x4223, 0x5102, 0x3304, 0x2315, 0x8157, 0x1146, 0x1326, 0x4223, 0x3304, 0x4223, 0x2315, 0x4223, 0x8157, 0x4223, 0x1146 },
	{ 0x4113, 0x8157, 0x1146, 0x5102, 0x4113, 0x1146 },
	{ 0x6201, 0x4113, 0x8157, 0x1146, 0x3304, 0x4113, 0x3304, 0x4113, 0x1146 },
	{ 0x2315, 0x8157, 0x1146, 0x5102, 0x6201, 0x8157, 0x6201, 0x8157, 0x5102 },
	{ 0x2315, 0x8157, 0x1146, 0x3304, 0x2315, 0x1146 },
	//208  0 5 12
	{ 0x2245, 0x3304, 0x1326, 0x8337, 0x8157, 0x3304, 0x8157, 0x3304, 0x8337 },
	{ 0x6201, 0x2245, 0x8157, 0x8337, 0x1326, 0x5102, 0x6201, 0x2245, 0x1326, 0x2245, 0x8337, 0x1326 },
	{ 0x2245, 0x3304, 0x1326, 0x8337, 0x8157, 0x6201, 0x2315, 0x4113, 0x3304, 0x8157, 0x3304, 0x8337 },
	{ 0x2245, 0x2315, 0x4113, 0x5102, 0x1326, 0x8337, 0x8157, 0x2245, 0x4113, 0x2245, 0x5102, 0x2245, 0x1326, 0x2245, 0x8337 },
	{ 0x4223, 0x8337, 0x8157, 0x2245, 0x3304, 0x5102, 0x4223, 0x2245, 0x3304, 0x4223, 0x8337, 0x2245 },
	{ 0x8157, 0x2245, 0x6201, 0x4223, 0x8337, 0x2245, 0x8337, 0x2245, 0x4223 },
	{ 0x2245, 0x3304, 0x5102, 0x4223, 0x8337, 0x8157, 0x4113, 0x6201, 0x2315, 0x2245, 0x8337, 0x2245, 0x4223, 0x3304, 0x4223 },
	{ 0x4223, 0x8337, 0x8157, 0x2245, 0x2315, 0x4113, 0x4223, 0x8157, 0x4223, 0x2245, 0x4223, 0x2315 },
	{ 0x4113, 0x8157, 0x2245, 0x3304, 0x1326, 0x4223, 0x4113, 0x8157, 0x1326, 0x8157, 0x3304, 0x1326 },
	{ 0x1326, 0x4223, 0x4113, 0x8157, 0x2245, 0x6201, 0x5102, 0x1326, 0x4113, 0x1326, 0x8157, 0x1326, 0x2245, 0x1326, 0x6201 },
	{ 0x8157, 0x2245, 0x3304, 0x1326, 0x4223, 0x6201, 0x2315, 0x8157, 0x3304, 0x8157, 0x1326, 0x8157, 0x4223, 0x8157, 0x6201 },
	{ 0x5102, 0x1326, 0x4223, 0x2315, 0x8157, 0x2245 },
	{ 0x3304, 0x5102, 0x4113, 0x8157, 0x2245, 0x5102, 0x2245, 0x5102, 0x8157 },
	{ 0x4113, 0x8157, 0x2245, 0x6201, 0x4113, 0x2245 },
	{ 0x5102, 0x6201, 0x2315, 0x8157, 0x2245, 0x3304, 0x5102, 0x2315, 0x5102, 0x8157, 0x5102, 0x2245 },
	{ 0x2315, 0x8157, 0x2245 },
	//224  0 10 12
	{ 0x1146, 0x1326, 0x8337, 0x2315, 0x2245, 0x1326, 0x2245, 0x1326, 0x2315 },
	{ 0x1146, 0x1326, 0x8337, 0x2315, 0x2245, 0x6201, 0x5102, 0x3304, 0x1326, 0x2245, 0x1326, 0x2315 },
	{ 0x6201, 0x2245, 0x1146, 0x1326, 0x8337, 0x4113, 0x6201, 0x2245, 0x8337, 0x2245, 0x1326, 0x8337 },
	{ 0x2245, 0x1146, 0x1326, 0x8337, 0x4113, 0x5102, 0x3304, 0x2245, 0x1326, 0x2245, 0x8337, 0x2245, 0x4113, 0x2245, 0x5102 },
	{ 0x5102, 0x1146, 0x2245, 0x2315, 0x8337, 0x4223, 0x5102, 0x1146, 0x8337, 0x1146, 0x2315, 0x8337 },
	{ 0x1146, 0x3304, 0x6201, 0x4223, 0x8337, 0x2315, 0x2245, 0x1146, 0x6201, 0x1146, 0x4223, 0x1146, 0x8337, 0x1146, 0x2315 },
	{ 0x8337, 0x4113, 0x6201, 0x2245, 0x1146, 0x5102, 0x4223, 0x8337, 0x6201, 0x8337, 0x2245, 0x8337, 0x1146, 0x8337, 0x5102 },
	{ 0x4223, 0x8337, 0x4113, 0x3304, 0x2245, 0x1146 },
	{ 0x4113, 0x2315, 0x2245, 0x1146, 0x1326, 0x4223, 0x4113, 0x1146, 0x1326, 0x4113, 0x2315, 0x1146 },
	{ 0x1146, 0x1326, 0x4223, 0x4113, 0x2315, 0x2245, 0x6201, 0x5102, 0x3304, 0x1146, 0x2315, 0x1146, 0x4113, 0x1326, 0x4113 },
	{ 0x1326, 0x4223, 0x6201, 0x2245, 0x1146, 0x4223, 0x1146, 0x4223, 0x2245 },
	{ 0x4223, 0x5102, 0x3304, 0x2245, 0x1146, 0x1326, 0x4223, 0x3304, 0x4223, 0x2245, 0x4223, 0x1146 },
	{ 0x2245, 0x1146, 0x5102, 0x4113, 0x2315, 0x1146, 0x2315, 0x1146, 0x4113 },
	{ 0x4113, 0x2315, 0x2245, 0x1146, 0x3304, 0x6201, 0x4113, 0x2245, 0x4113, 0x1146, 0x4113, 0x3304 },
	{ 0x6201, 0x2245, 0x1146, 0x5102, 0x6201, 0x1146 },
	{ 0x3304, 0x2245, 0x1146 },
	//240  1 2 4 8
	{ 0x3304, 0x1326, 0x8337, 0x2315, 0x3304, 0x8337 },
	{ 0x5102, 0x1326, 0x8337, 0x2315, 0x6201, 0x1326, 0x6201, 0x1326, 0x2315 },
	{ 0x6201, 0x3304, 0x1326, 0x8337, 0x4113, 0x3304, 0x4113, 0x3304, 0x8337 },
	{ 0x5102, 0x1326, 0x8337, 0x4113, 0x5102, 0x8337 },
	{ 0x4223, 0x8337, 0x2315, 0x3304, 0x5102, 0x8337, 0x5102, 0x8337, 0x3304 },
	{ 0x6201, 0x4223, 0x8337, 0x2315, 0x6201, 0x8337 },
	{ 0x3304, 0x5102, 0x4223, 0x8337, 0x4113, 0x6201, 0x3304, 0x4223, 0x3304, 0x8337, 0x3304, 0x4113 },
	{ 0x4113, 0x4223, 0x8337 },
	{ 0x4113, 0x2315, 0x3304, 0x1326, 0x4223, 0x2315, 0x4223, 0x2315, 0x1326 },
	{ 0x1326, 0x4223, 0x4113, 0x2315, 0x6201, 0x5102, 0x1326, 0x4113, 0x1326, 0x2315, 0x1326, 0x6201 },
	{ 0x3304, 0x1326, 0x4223, 0x6201, 0x3304, 0x4223 },
	{ 0x5102, 0x1326, 0x4223 },
	{ 0x5102, 0x4113, 0x2315, 0x3304, 0x5102, 0x2315 },
	{ 0x6201, 0x4113, 0x2315 },
	{ 0x6201, 0x3304, 0x5102 },
	{}
	//256
};

const Vector3 marching_cube_vertices[8] = {
	Vector3(0, 0, 0),
	Vector3(1, 0, 0),
	Vector3(0, 0, 1),
	Vector3(1, 0, 1),
	Vector3(0, 1, 0),
	Vector3(1, 1, 0),
	Vector3(0, 1, 1),
	Vector3(1, 1, 1)
};

} // namespace MarchingCubes