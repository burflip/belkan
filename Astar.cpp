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
#include <cmath>
#include <cstdlib>

using namespace std;

Astar::Astar() {
	this->ignore_better_choices = false;
}

Astar::~Astar() {
	// TODO Auto-generated destructor stub
}

Astar::Astar(char mapa_entorno_[][MAP_MAX_SIZE], char mapa_objetos_[][MAP_MAX_SIZE], pair<int, int> origin, pair<int, int> goal, bool ignore_better_choices) {
	this->goal = goal;
	this->origin = origin;
	this->ignore_better_choices = ignore_better_choices;
	translate_map(mapa_entorno_, mapa_objetos_);
}

int Astar::heuristic(pair<int, int> coords) {
	return abs(coords.first - goal.first) + abs(coords.second - goal.second);
}

pair<bool,bool> Astar::solve() {
	bool solution = false;
	bool there_is_solution = true;
	bool goal_recalculated = false;
	Node n = Node(make_pair(origin.first, origin.second), heuristic(origin));
	nodes.insert(pair<pair<int, int>, Node>(origin, n));
	this->actual_node_coords = origin;
	closed.push_back(origin);
	calculateAndOpenAdjacents(origin);
	while (!solution && there_is_solution) {
		if (open.size() == 0) {
			there_is_solution = false;
		} else {
			findBestOpenedChoiceAndCloseIt();
			if (this->actual_node_coords == goal) {
				solution = true;
				buildSolution();
			} else {
				if (calculateAndOpenAdjacents(this->actual_node_coords)) {
					this->goal = this->actual_node_coords;
					solution = true;
					goal_recalculated = true;
					buildSolution();
				}
			}
		}
	}
	return pair<bool,bool>(there_is_solution,goal_recalculated);
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
			may_recalculate = createAndInsertNode(coords, new_node_coordinates);
			break;
		case 1:
			new_node_coordinates = pair<int, int>(coords.first + 1, coords.second);
			may_recalculate = createAndInsertNode(coords, new_node_coordinates);
			break;
		case 2:
			new_node_coordinates = pair<int, int>(coords.first, coords.second + 1);
			may_recalculate = createAndInsertNode(coords, new_node_coordinates);
			break;
		case 3:
			new_node_coordinates = pair<int, int>(coords.first - 1, coords.second);
			may_recalculate = createAndInsertNode(coords, new_node_coordinates);
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
				if(!this->ignore_better_choices)
					new_goal = true; // Cuando se encuentra una zona desconocida del mapa en la búsqueda del trayecto, cambiar goal.
				is_ok = true;
			} else if (the_map[coords] == '-') {
				is_ok = true;
			}
		}
	}
	return make_pair(is_ok, new_goal);
}

bool Astar::createAndInsertNode(pair<int, int> parent_coordinates, pair<int, int> coordinates) {
	pair<bool, bool> checked = checkCoordinates(coordinates);
	if (checked.first) {
		int g_cost = nodes[parent_coordinates].getG() + 10;
		Node n = Node(pair<int, int>(coordinates.first, coordinates.second), heuristic(coordinates), g_cost);
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
		if (checked.second) {
			closed.push_back(coordinates);
			this->actual_node_coords = coordinates;
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
	pair<int, int> current_coordinates = nodes[goal].getCoords();
	//cout << "SOLUTION:" << endl;
	while (current_coordinates != origin) {
		//cout << current_coordinates.first << " " << current_coordinates.second << ", ";
		current_coordinates = nodes[current_coordinates].getParentCoords();
		solution.push_front(current_coordinates);
	}
	solution.pop_front();
	/**
	cout << endl;
	cout << "SOLUTION SIZE: " << solution.size() << endl;
	cout << endl;
	**/
}

void Astar::printRoute() {
	cout << "Ruta escogida desde " << "(" << origin.first << "," << origin.second << "):"<< endl;
	deque<pair<int,int> >::iterator it = solution.begin();
	while(it != solution.end()) {
		cout << "(" << (*it).first  << "," << (*it).second << "), ";
		it ++;
	}
	cout << endl;
}

void Astar::printPartialMap()
{
	vector<vector<char>> matrix;
	for(unsigned int i=0;i<solution.size()*2;i++) {
		vector<char> tmp_vec;
		for(unsigned int j=0;j<solution.size()*2;j++){
			if(i==solution.size() && j==solution.size()) {
				tmp_vec.push_back('O');
			} else if(the_map[make_pair(j+origin.first,i+origin.second)] != 'x') {
				tmp_vec.push_back('-');
			} else {
				tmp_vec.push_back('s');
			}
		}
		matrix.push_back(vector<char>(tmp_vec));
	}
	for(unsigned int i=0;i<solution.size()*2;i++) {
		for(unsigned int j=0;j<solution.size()*2;j++){
			deque<pair<int,int> >::iterator it = solution.begin();
			bool printed = false;
			while(it != solution.end()) {
				if(((*it).first-origin.first+solution.size()-1) == j && ((*it).second-origin.second+solution.size()-1) == i) {
					cout << "X ";
					printed = true;
				}
				it ++;
			}
			if(!printed)
				cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Matrix: " << solution.size() << endl;
}
