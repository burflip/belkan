/*
 * Node.h
 *
 *  Created on: 8 de may. de 2016
 *      Author: valentin
 */

#ifndef NODE_H_
#define NODE_H_

#include <utility>

using namespace std;

class Node {
public:
	Node();
	virtual ~Node();
	Node(pair<int,int> coords, int h_value, int g_value=0);
	void setParent(Node &n);

	inline int getG() const {
		return g_value;
	}

	inline void setG(int value) {
		g_value = value;
	}

	inline int getH() const {
		return h_value;
	}

	inline int getF() const {
		return f_value;
	}

	inline void calculateF(){
		f_value = g_value + h_value;
	}

	inline void setH(int value) {
		h_value = value;
	}

	inline const pair<int, int>& getParentCoords() const {
		return parent_coords;
	}

	inline const pair<int, int>& getCoords() const {
		return coords;
	}

private:
	int h_value, g_value, f_value;
	pair<int,int> coords;
	pair<int,int> parent_coords;
};

#endif /* NODE_H_ */
