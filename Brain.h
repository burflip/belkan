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
#include "Memory.h"
using namespace std;

#define SIGHT_LENGTH 10

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
	void stayAlert(Agent &agent);
	void remember(pair<int,int> coords, Memory token);
	bool priorize(Agent &agent);
	void meditate();
	bool checkSurface(char surface_item, pair<int, int> agent_coords, pair<int, int> coordinates);
	pair<int,int> translateSensorToCoords(int orientation, int sensor_position, pair<int,int> agent_coords);
	void iSeePK(pair<int,int> agent_coords,pair<int,int> PK_location);
	void iSeeSomething(char item, pair<int, int> agent_coords, pair<int, int> PK_location);
	void iHaveSomethingAhead(char item, Agent &agent);
	void iAmOnSomething(char item, Agent &agent);
	void iHavePKAhead();
	void iAmOnAPK(Agent &agent);

	void yellWhatImDoing(pair<int,int> agent_coords);

private:
	queue<ActionType> current_path;
	queue<char> mochila;
	pair<int, int> current_goal;
	bool in_path;
	int steps;
	pair<int,int> first_pk;
	pair<int,int> second_pk;
	bool map_oriented;
	map<pair<int,int>, Memory> memories;
	map<pair<int,int>, Memory> objectives;
	vector<char> important_surface = {'K'};
	vector<char> important_objects = {};
	pair<int,int> current_objective;
};

#endif /* BRAIN_H_ */
