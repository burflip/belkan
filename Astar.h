/*
 * Astar.h
 *
 *  Created on: 8 de may. de 2016
 *      Author: valentin
 */

#ifndef ASTAR_H_
#define ASTAR_H_
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <deque>
#include "Node.h"

#define MAP_MAX_SIZE 200

using namespace std;

class Astar {
public:
	virtual ~Astar();
	Astar();
	Astar(char mapa_entorno_[][MAP_MAX_SIZE], char mapa_objetos_[][MAP_MAX_SIZE], pair<int, int> origin, pair<int, int> goal, bool ignore_better_choices = false);
	int heuristic(pair<int, int> coords);
	pair<bool,bool> solve();
	void assingActual(pair<int, int> coords);
	bool calculateAndOpenAdjacents(pair<int, int> coords);
	inline deque<pair<int, int> > & getSolution() {
		return solution;
	}
	pair<bool,bool> checkCoordinates(pair<int, int> coords);

	inline pair<int, int> getGoal() const {
		return goal;
	}
	void printRoute();
	void printPartialMap();

private:
	void translate_map(char mapa_entorno_[][MAP_MAX_SIZE], char mapa_surface_[][MAP_MAX_SIZE]);
	bool createAndInsertNode(pair<int, int> parent_coordinates, pair<int, int> coordinates);
	pair<int,int> findBestOpenedChoiceAndCloseIt();
	void buildSolution();
	map<pair<int, int>, char> the_map;
	map<pair<int, int>, Node> nodes;
	vector<pair<int, int> > open;
	vector<pair<int, int> > closed;
	deque<pair<int, int> > solution;
	pair<int, int> goal;
	pair<int, int> origin;
	pair<int, int> actual_node_coords;
	bool ignore_better_choices;
};

#endif /* ASTAR_H_ */
