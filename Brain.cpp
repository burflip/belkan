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
}

Brain::Brain(Agent &agent) {
	int x, y, orientacion;
	agent.GetCoord(x, y, orientacion);
	cout << x << " " << y << endl;
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
	int x, y, orientacion;
	getGoal(agent);
	agent.GetCoord(x, y, orientacion);
	Astar astar_alg = Astar(agent.mapa_entorno_,agent.mapa_objetos_,pair<int,int>(x,y), current_goal);
	astar_alg.solve();
}
