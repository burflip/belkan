/*
 * Brain.cpp
 *
 *  Created on: 8 de may. de 2016
 *      Author: valentin
 */

#include "Brain.h"
#include <cmath>
#include <utility>
#include "belkan.h"
#include "Astar.h"

using namespace std;
Brain::Brain() {
	in_path = false;
	pasos=0;
}

Brain::Brain(Agent &agent) {
	in_path = false;
	pasos=0;
}

Brain::~Brain() {
	// TODO Auto-generated destructor stub
}

void Brain::getGoal(Agent &agent) {
	bool goal_found = false;
	double current_distance = 1000000;
	int goal_x, goal_y;
	int x, y, orientacion;
	agent.GetCoord(x, y, orientacion);
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			if (agent.mapa_entorno_[i][j] == '?') {
				double d = sqrt(pow(i - x, 2) + pow(j - y, 2));
				if (d < current_distance) {
					goal_found = true;
					goal_x = j;
					goal_y = i;
					current_distance = d;
				}
			}
		}
	}
	if (goal_found) {
		this->current_goal = make_pair(goal_x, goal_y);
	}
	cout << current_goal.first << " " << current_goal.second << endl;
}

Brain::ActionType Brain::Think(Agent &agent) {
	Brain::ActionType next_move;
	do {
		if (!in_path) {
			int x, y, orientation;
			agent.GetCoord(y, x, orientation);
			pair<int, int> coords = pair<int, int>(x, y);
			translateToMoves(coords, orientation, calculateGoalAndPath(agent));
			//tellCurrentPath();
			in_path = true;
		}

		if (current_path.empty()) {
			in_path = false;
			next_move = Brain::ActionType::actIDLE;
		} else {
			next_move = current_path.front();
			current_path.pop();
		}
	} while (next_move == Brain::ActionType::actIDLE);
	pasos++;
	cout << "P: " << pasos << endl;
	return next_move;
}

void Brain::translateToMoves(pair<int, int> &agent_coords, int &orientation, deque<pair<int, int> > path) {
	deque<pair<int, int> >::iterator it = path.begin();
	pair<int, int> partial_coords = pair<int, int>(agent_coords.first, agent_coords.second);
	while (it != path.end()) {
		vector<Brain::ActionType> partial_path = lookTo(partial_coords, orientation, *it);
		partial_path.push_back(Brain::ActionType::actFORWARD);
		vector<Brain::ActionType>::iterator partial_it = partial_path.begin();
		while (partial_it != partial_path.end()) {
			current_path.push(*partial_it);
			partial_it++;
		}
		it++;
	}
	vector<Brain::ActionType> partial_path = lookTo(partial_coords, orientation, current_goal);
	vector<Brain::ActionType>::iterator partial_it = partial_path.begin();
	while (partial_it != partial_path.end()) {
		current_path.push(*partial_it);
		partial_it++;
	}

}

vector<Brain::ActionType> Brain::lookTo(pair<int, int> &agent_coords, int &orientation, pair<int, int> coords) {
	vector<Brain::ActionType> move_list;
	if (agent_coords.first < coords.first) {
		switch (orientation) {
		case 0:
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		case 2:
			move_list.push_back(Brain::ActionType::actTURN_L);
			break;
		case 3:
			move_list.push_back(Brain::ActionType::actTURN_R);
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		}
		orientation = 1;
	} else if (agent_coords.first > coords.first) {
		switch (orientation) {
		case 2:
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		case 0:
			move_list.push_back(Brain::ActionType::actTURN_L);
			break;
		case 1:
			move_list.push_back(Brain::ActionType::actTURN_R);
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		}
		orientation = 3;
	} else if (agent_coords.second > coords.second) {
		switch (orientation) {
		case 1:
			move_list.push_back(Brain::ActionType::actTURN_L);
			break;
		case 3:
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		case 2:
			move_list.push_back(Brain::ActionType::actTURN_R);
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		}
		orientation = 0;
	} else if (agent_coords.second < coords.second) {
		switch (orientation) {
		case 3:
			move_list.push_back(Brain::ActionType::actTURN_L);
			break;
		case 1:
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		case 0:
			move_list.push_back(Brain::ActionType::actTURN_R);
			move_list.push_back(Brain::ActionType::actTURN_R);
			break;
		}
		orientation = 2;
	}
	agent_coords = coords;
	return move_list;
}

void Brain::tellCurrentPath() {
	deque<Brain::ActionType> deq_tmp;
	while(!current_path.empty()) {
		deq_tmp.push_back(current_path.front());
		current_path.pop();
	}
	deque<Brain::ActionType>::iterator it = deq_tmp.begin();
	cout << "S:" << current_path.size() << ",P=>";
	while (it != deq_tmp.end()) {
		switch (*it) {
		case Brain::ActionType::actFORWARD:
			cout << "FW,";
			break;
		case Brain::ActionType::actTURN_L:
			cout << "TL,";
			break;
		case Brain::ActionType::actTURN_R:
			cout << "TR,";
			break;
		default:
			cout << "Other";
			break;
		}
		it++;
	}
	cout << endl;while(!current_path.empty()) {
		deq_tmp.push_back(current_path.front());
		current_path.pop();
	}
	it = deq_tmp.begin();
	while(it != deq_tmp.end()) {
		current_path.push(*it);
		it++;
	}
}

deque<pair<int, int> > Brain::calculateGoalAndPath(Agent& agent) {
	int x, y, orientation;
	pair<bool, bool> solved;
	getGoal(agent);
	deque<pair<int, int> > path;
	agent.GetCoord(y, x, orientation);
	//cout << "-----------------------------" << endl;
	//cout << "AGENTE: " << x << "," << y << ". GOAL: " << current_goal.first << "," << current_goal.second << endl;
	Astar astar_alg = Astar(agent.mapa_entorno_, agent.mapa_objetos_, pair<int, int>(x, y), current_goal);
	solved = astar_alg.solve();
	if (solved.second) {
		this->current_goal = astar_alg.getGoal();
	}
	if (!solved.first) {
		cout << "ERROR: no se me ocurre una forma de descubrir más mapa!" << endl;
		cout << "PASOS: "<< pasos << endl;
		exit(0);
	}
	//cout << "AGENTE: " << x << "," << y << ". GOAL: " << current_goal.first << "," << current_goal.second << endl;
	//cout << "-----------------------------" << endl;
	path = deque<pair<int, int> >(astar_alg.getSolution());
	//astar_alg.printRoute();
	return path;
}
