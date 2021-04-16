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

#include "terraman_level_generator_flat.h"

#include "../world/terra_chunk.h"

int TerramanLevelGeneratorFlat::get_floor_position() const {
	return _floor_position;
}
void TerramanLevelGeneratorFlat::set_floor_position(const int floor_height) {
	_floor_position = floor_height;
}

Dictionary TerramanLevelGeneratorFlat::get_channel_map() {
	return _channel_map;
}
void TerramanLevelGeneratorFlat::set_channel_map(const Dictionary &map) {
	_channel_map = map;
}

void TerramanLevelGeneratorFlat::_generate_chunk(Ref<TerraChunk> chunk) {
	const Variant *key = NULL;
	while ((key = _channel_map.next(key))) {
		int k = *key;
		int value = _channel_map[*key];

		chunk->channel_fill(value, k);
	}
}

TerramanLevelGeneratorFlat::TerramanLevelGeneratorFlat() {
	_floor_position = 0;
}

TerramanLevelGeneratorFlat::~TerramanLevelGeneratorFlat() {
	_channel_map.clear();
}

void TerramanLevelGeneratorFlat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_floor_position"), &TerramanLevelGeneratorFlat::get_floor_position);
	ClassDB::bind_method(D_METHOD("set_floor_position", "value"), &TerramanLevelGeneratorFlat::set_floor_position);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "floor_position"), "set_floor_position", "get_floor_position");

	ClassDB::bind_method(D_METHOD("get_channel_map"), &TerramanLevelGeneratorFlat::get_channel_map);
	ClassDB::bind_method(D_METHOD("set_channel_map", "value"), &TerramanLevelGeneratorFlat::set_channel_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "channel_map"), "set_channel_map", "get_channel_map");

	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &TerramanLevelGeneratorFlat::_generate_chunk);
}
