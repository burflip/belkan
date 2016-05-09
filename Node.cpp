/*
 * Node.cpp
 *
 *  Created on: 8 de may. de 2016
 *      Author: valentin
 */

#include "Node.h"
#include <iostream>

using namespace std;

Node::Node() {
	this->h_value = 0;
	this->g_value = 0;
	this->f_value = 0;
	parent_coords = make_pair(99,99);
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

Node::Node(pair<int,int> coords, int h_value, int g_value) {
	this->coords = make_pair(coords.first,coords.second);
	this->h_value = h_value;
	this->g_value = g_value;
	this->f_value = h_value+g_value;
	parent_coords = coords;
}

void Node::setParent(Node& n) {
	parent_coords = n.getCoords();
}
