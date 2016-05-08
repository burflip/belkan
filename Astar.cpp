/*
 * Astar.cpp
 *
 *  Created on: 8 de may. de 2016
 *      Author: valentin
 */

#include "Astar.h"
#include "Node.h"
#include <climits>
#include <vector>
#include <deque>
#include <iostream>

using namespace std;

Astar::Astar() {
}

Astar::~Astar() {
	// TODO Auto-generated destructor stub
}

Astar::Astar(char mapa_entorno_[][MAP_MAX_SIZE], char mapa_objetos_[][MAP_MAX_SIZE], pair<int, int> origin, pair<int, int> goal) {
	this->goal = goal;
	this->origin = origin;
	translate_map(mapa_entorno_, mapa_objetos_);
}

int Astar::heuristic(pair<int, int> coords) {
	return abs(coords.first - goal.first) + abs(coords.second - goal.second);
}

void Astar::solve() {
	bool solution = false;
	Node n = Node(make_pair(origin.first, origin.second), heuristic(origin));
	nodes.insert(pair<pair<int, int>, Node>(origin, n));
	closed.push_back(origin);
	calculateAndOpenAdjacents(origin);
	while (!solution) {
		findBestOpenedChoiceAndCloseIt();
		if (this->actual_node_coords == goal) {
			cout << "ASTAR" << endl;
			cout << actual_node_coords.first << " " << actual_node_coords.second << endl;
			cout << goal.first << " " << goal.second << endl;
			cout << nodes[actual_node_coords].getCoords().first << " " << nodes[actual_node_coords].getCoords().second << endl;
			solution = true;
			buildSolution();
		} else {
			calculateAndOpenAdjacents(this->actual_node_coords);
		}
	}
}

void Astar::assingActual(pair<int, int> coords) {
	if (nodes.find(coords) != nodes.end()) {
		this->actual_node_coords = coords;
	}
}

bool Astar::calculateAndOpenAdjacents(pair<int, int> coords) {
	pair<int, int> new_node_coordinates;
	bool may_recalculate = false;
	for (int i = 0; i < 4 && !may_recalculate; i++) {
		switch (i) {
		case 0:
			new_node_coordinates = pair<int, int>(coords.first, coords.second - 1);
			may_recalculate = createAndInsertNode(new_node_coordinates);
			break;
		case 1:
			new_node_coordinates = pair<int, int>(coords.first + 1, coords.second);
			may_recalculate = createAndInsertNode(new_node_coordinates);
			break;
		case 2:
			new_node_coordinates = pair<int, int>(coords.first, coords.second + 1);
			may_recalculate = createAndInsertNode(new_node_coordinates);
			break;
		case 3:
			new_node_coordinates = pair<int, int>(coords.first - 1, coords.second);
			may_recalculate = createAndInsertNode(new_node_coordinates);
			break;
		}
	}
	return may_recalculate;
}

/**
 * (is_ok to create node, new goal in map: new hidden zone)
 */
pair<bool, bool> Astar::checkCoordinates(pair<int, int> coords) {
	bool is_ok = false;
	bool new_goal = false;
	if (coords.first >= 0 && coords.second >= 0 && coords.first <= MAP_MAX_SIZE && coords.second <= MAP_MAX_SIZE) {
		if (the_map.find(coords) != the_map.end()) {
			if (the_map[coords] == '?') {
				new_goal = true; //TODO: cuando se encuentra una zona desconocida del mapa en la búsqueda del trayecto, cambiar goal.
			} else if (the_map[coords] == '-') {
				is_ok = true;
			}
		}
	}
	return make_pair(is_ok, new_goal);
}

bool Astar::createAndInsertNode(pair<int, int> coordinates) {
	pair<bool, bool> checked = checkCoordinates(coordinates);
	if (checked.first) {
		Node n = Node(pair<int,int>(coordinates.first,coordinates.second), heuristic(coordinates), 10);
		n.calculateF();
		n.setParent(nodes[this->actual_node_coords]);
		pair<map<pair<int, int>, Node>::iterator, bool> can_insert = nodes.insert(pair<pair<int, int>, Node>(coordinates, n));
		if (can_insert.second == true) {
			open.push_back(coordinates);
		} else {
			if (n.getF() < (*can_insert.first).second.getF()) {
				nodes[coordinates] = n;
			}
		}
	}
	return checked.second;
}

void Astar::translate_map(char mapa_entorno_[][MAP_MAX_SIZE], char mapa_surface_[][MAP_MAX_SIZE]) {
	for (int i = 0; i < MAP_MAX_SIZE; i++) {
		for (int j = 0; j < MAP_MAX_SIZE; j++) {
			if ((mapa_entorno_[i][j] == 'S' || mapa_entorno_[i][j] == 'T' || mapa_entorno_[i][j] == '?')
					&& (mapa_surface_[i][j] <= 'a' || mapa_surface_[i][j] >= 'z')) {
				if (mapa_entorno_[i][j] == '?') {
					the_map[make_pair(i, j)] = '?';
				} else {
					the_map[make_pair(i, j)] = '-';
				}
			} else {
				the_map[make_pair(i, j)] = 'x';
			}
		}
	}

}

pair<int, int> Astar::findBestOpenedChoiceAndCloseIt() {
	vector<pair<int, int> >::iterator best_pos, it = open.begin();
	pair<int, int> best_node_coordinates;
	int best_f = INT_MAX;
	while (it != open.end()) {
		int current_f = nodes[(*it)].getF();
		if (current_f < best_f) {
			best_f = current_f;
			best_node_coordinates = *it;
			best_pos = it;
		}
		it++;
	}

	open.erase(best_pos);
	closed.push_back(best_node_coordinates);
	actual_node_coords = best_node_coordinates;
	return actual_node_coords;
}

void Astar::buildSolution() {
	pair<int,int> current_coordinates = nodes[goal].getCoords();
	cout << "SOLUTION:" << endl;
	while(current_coordinates != origin && (current_coordinates.first !=0 && current_coordinates.second != 0) ) {
		cout << current_coordinates.first << " " << current_coordinates.second << " ";
 		solution.push_front(current_coordinates);
 		current_coordinates = nodes[current_coordinates].getParentCoords();
	}
	cout << endl;
	cout << "SOLUTION SIZE: " << solution.size() << endl;
}
