/*
 * Memory.cpp
 *
 *  Created on: 10 de may. de 2016
 *      Author: valentin
 */

#include "Memory.h"

Memory::Memory() {
	this->location = pair<int, int>(-1, -1);
	this->is_key = false;
	this->is_lock = false;
	this->item = '?';
}

Memory::~Memory() {
	// TODO Auto-generated destructor stub
}

Memory::Memory(char item, pair<int, int> location, bool is_key, bool is_lock) {
	this->location = location;
	this->is_key = is_key;
	this->is_lock = is_lock;
	this->item = item;
}
