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
#include <string>

using namespace std;
Brain::Brain() {
	in_path = false;
	steps = 0;
	map_oriented = false;
	first_pk = pair<int, int>(-1, -1);
	second_pk = pair<int, int>(-1, -1);
}

Brain::Brain(Agent &agent) {
	in_path = false;
	steps = 0;
	map_oriented = false;
	first_pk = pair<int, int>(-1, -1);
	second_pk = pair<int, int>(-1, -1);
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
	//cout << "GOAL: " << current_goal.first << " " << current_goal.second << endl;
}

Brain::ActionType Brain::Think(Agent &agent) {
	Brain::ActionType next_move;
	do {
		stayAlert(agent);
		meditate();
		tellCurrentPath();
		if (priorize(agent)) {
			in_path = true;
			tellCurrentPath();
		}
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
	steps++;
	cout << "FPK:" << first_pk.first << " " << first_pk.second << endl;
	cout << "SPK:" << second_pk.first << " " << second_pk.second << endl;
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
	while (!current_path.empty()) {
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
	cout << endl;
	while (!current_path.empty()) {
		deq_tmp.push_back(current_path.front());
		current_path.pop();
	}
	it = deq_tmp.begin();
	while (it != deq_tmp.end()) {
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
	//yellWhatImDoing(agent.getCoord());
	Astar astar_alg = Astar(agent.mapa_entorno_, agent.mapa_objetos_, pair<int, int>(x, y), current_goal);
	solved = astar_alg.solve();
	if (solved.second) {
		this->current_goal = astar_alg.getGoal();
	}
	if (!solved.first) {
		cout << "ERROR: no se me ocurre una forma de descubrir más mapa!" << endl;
		cout << "PASOS: " << steps << endl;
		//exit(0);
	}
	//yellWhatImDoing(agent.getCoord());
	//cout << "-----------------------------" << endl;
	path = deque<pair<int, int> >(astar_alg.getSolution());
	//astar_alg.printRoute();
	return path;
}

void Brain::stayAlert(Agent& agent) {
	int x, y, orientation;
	agent.GetCoord(y, x, orientation);
	pair<int, int> coords = pair<int, int>(x, y);
	for (int i = 1; i < SIGHT_LENGTH; i++) {
		//Check important_surface
		pair<int, int> item_coords = translateSensorToCoords(orientation, i, coords);
		checkSurface(agent.whatISeeThere(i).first, coords, item_coords);
		checkSurface(agent.whatISeeThere(i).second, coords, item_coords);
	}
}

void Brain::remember(pair<int, int> coords, Memory token) {
}

bool Brain::priorize(Agent& agent) {
	bool there_is_something_there = false;
	map<pair<int,int>,Memory>::iterator it = objectives.begin();
	if (!this->objectives.empty()) {
		int x, y, orientation;
		pair<bool, bool> solved;
		getGoal(agent);
		deque<pair<int, int> > path;
		agent.GetCoord(y, x, orientation);
		pair<int, int> coords = pair<int, int>(x, y);
		map<pair<int, int>, Memory>::iterator it = this->objectives.begin();

		this->current_goal = (*it).first;
		bool iHaveItTouchingMaNose = (current_path.empty() && !this->objectives.empty()
				&& (agent.whatISeeThere(1).first == (*it).second.getItem() || agent.whatISeeThere(1).second == (*it).second.getItem()));
		bool iAmOnIt = (current_path.empty() && current_goal.first == x && current_goal.second == y);

		if (iAmOnIt) {
			//cout << "I AM ON IT" << endl;
			iAmOnSomething((*it).second.getItem(), agent);
		} else if (iHaveItTouchingMaNose) {
			//cout << "I HAVE IT IN NOSE" << endl;
			iHaveSomethingAhead((*it).second.getItem(), agent);
		} else {
			cout << "I HAVE A MISION: " << endl;
			//yellWhatImDoing(agent.getCoord());
			Astar astar_alg = Astar(agent.mapa_entorno_, agent.mapa_objetos_, coords, current_goal, true); //Cuidado, movimiento forzado
			solved = astar_alg.solve();
			if (solved.second) {
				this->current_goal = astar_alg.getGoal();
			}
			if (!solved.first) {
				cout << "ERROR: no se me ocurre una forma de encontrar lo que buscas, ni puedo encontrar más mapa!" << endl;
				cout << "PASOS: " << steps << endl;
				//exit(0);
			}
			while (!current_path.empty()) {
				current_path.pop();
			}
			path = deque<pair<int, int> >(astar_alg.getSolution());
			translateToMoves(coords, orientation, path);
		}
		there_is_something_there = true;
	}
	return there_is_something_there;
}

void Brain::meditate() {
}

bool Brain::checkSurface(char surface_item, pair<int, int> agent_coords, pair<int, int> coordinates) {
	vector<char>::const_iterator it = important_surface.cbegin();
	bool something_found = false;
	while (it != important_surface.cend()) {
		if (*it == surface_item) {
			iSeeSomething(surface_item, agent_coords, coordinates);
			something_found = true;
		}
		it++;
	}
	return something_found;
}

pair<int, int> Brain::translateSensorToCoords(int orientation, int sensor_position, pair<int, int> agent_coords) {
	int increment_main = 0, increment_second = 0;
	pair<int, int> coords;
	if (sensor_position == 1) {
		increment_main = 1;
	} else if (sensor_position > 1 && sensor_position <= 4) {
		increment_main = 2;
		increment_second = sensor_position - 3;
	} else if (sensor_position > 4) {
		increment_main = 3;
		increment_second = sensor_position - 7;
	}
	switch (orientation) {
	case 0:
		coords = pair<int, int>(agent_coords.first + increment_second, agent_coords.second - increment_main);
		break;
	case 1:
		coords = pair<int, int>(agent_coords.first + increment_main, agent_coords.second + increment_second);
		break;
	case 2:
		coords = pair<int, int>(agent_coords.first - increment_second, agent_coords.second + increment_main);
		break;
	case 3:
		coords = pair<int, int>(agent_coords.first - increment_main, agent_coords.second - increment_second);
		break;
	}
	return coords;
}

void Brain::iSeePK(pair<int, int> agent_coords, pair<int, int> PK_location) {
	if ((this->first_pk.first == -1 || this->second_pk.first == -1) && first_pk != PK_location) {
		Memory PK = Memory('K', PK_location, true, true);
		this->objectives[PK_location] = PK;
	}
}

void Brain::iSeeSomething(char item, pair<int, int> agent_coords, pair<int, int> PK_location) {
	switch (item) {
	case 'K':
		iSeePK(agent_coords, PK_location);
		break;
	}
}

void Brain::iHaveSomethingAhead(char item, Agent& agent) {
	switch (item) {
	case 'K':
		iHavePKAhead();
		break;
	}
}

void Brain::iAmOnSomething(char item, Agent& agent) {
	switch (item) {
	case 'K':
		iAmOnAPK(agent);
		break;
	}
}

void Brain::iHavePKAhead() {
	current_path.push(Brain::ActionType::actFORWARD);
}

void Brain::iAmOnAPK(Agent& agent) {
	string msg = agent.whatIHear();
	int second_space, third_space, fourth_space, point;
	second_space = msg.find(' ', msg.find(' ') + 1);
	third_space = msg.find(' ', second_space + 1);
	fourth_space = msg.find(' ', third_space + 1);
	point = msg.find('.', fourth_space);
	int x = atoi(msg.substr(second_space, (third_space - second_space)).c_str());
	int y = atoi(msg.substr(fourth_space, (point - fourth_space)).c_str());
	if (this->first_pk.first == -1) {
		this->first_pk = pair<int, int>(x, y);
	} else if (this->first_pk != pair<int, int>(x, y)) {
		this->second_pk = pair<int, int>(x, y);
	}
	map<pair<int, int>, Memory>::iterator it = this->objectives.find(agent.getCoord());
	if (it != this->objectives.end()) {
		this->objectives.clear();
	}
}

void Brain::yellWhatImDoing(pair<int, int> agent_coords) {
	cout << "AGENTE: " << agent_coords.first << "," << agent_coords.second << ". GOAL: " << current_goal.first << "," << current_goal.second << endl;
}
