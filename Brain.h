/*
 * Brain.h
 *
 *  Created on: 8 de may. de 2016
 *      Author: valentin
 */

#ifndef BRAIN_H_
#define BRAIN_H_

#include <deque>
#include <queue>
#include <utility>
#include "Astar.h"
using namespace std;

class Agent;
class Brain {
public:

	enum class ActionType {
		actFORWARD, // avanzar
		actTURN_L,  // Girar Izquierda
		actTURN_R,  // Girar Derecha
		actIDLE,    // No hacer nada
		actPICKUP,  // Recoger un objeto
		actPUTDOWN, // Soltar un objeto
		actPUSH,    // Meter en la mochila
		actPOP,     // Sacar de la mochila
		actGIVE,	// Dar un objeto a un personaje
		actTHROW	// Lanzar un objeto
	};

	Brain();
	Brain(Agent &a);
	virtual ~Brain();
	void getGoal(Agent &agent);

	ActionType Think(Agent &agent);
	void translateToMoves(pair<int, int> &agent_coords, int &orientation, deque<pair<int,int> > path);
	vector<ActionType> lookTo(pair<int,int> &agent_coords, int &orientation, pair<int, int> coords);
	void tellCurrentPath();
	deque<pair<int,int> > calculateGoalAndPath(Agent &agent);

private:
	queue<ActionType> current_path;
	queue<char> mochila;
	pair<int, int> current_goal;
	bool in_path;
	int pasos;

};

#endif /* BRAIN_H_ */
