/*
 * Memory.h
 *
 *  Created on: 10 de may. de 2016
 *      Author: valentin
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include<utility>

using namespace std;

class Memory {
public:
	Memory();
	Memory(char item, pair<int, int> location, bool is_key, bool is_lock);
	virtual ~Memory();

	bool isIsKey() const {
		return is_key;
	}

	void setIsKey(bool isKey) {
		is_key = isKey;
	}

	bool isIsLock() const {
		return is_lock;
	}

	void setIsLock(bool isLock) {
		is_lock = isLock;
	}

	const pair<int, int>& getLocation() const {
		return location;
	}

	void setLocation(const pair<int, int>& location) {
		this->location = location;
	}

	const pair<int, int>& getRealLocation() const {
		return real_location;
	}

	void setRealLocation(const pair<int, int>& realLocation) {
		real_location = realLocation;
	}

	char getItem() const {
		return item;
	}

private:
	pair<int, int> location;
	bool is_key;
	bool is_lock;
	pair<int,int> real_location;
	char item;

};

#endif /* MEMORY_H_ */
