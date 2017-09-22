/*
 * State.h
 *
 *  Created on: Sep 20, 2017
 *      Author: adilhamidmalla
 */

#ifndef STATE_H_
#define STATE_H_

#include <stack>

class State {
private:
	std::stack<char> *stackHolders;
	int numOfHolders;

public:
	State();
	State(int);
	float heuristicsEstimate(State goalState);
	virtual ~State();
};

#endif /* STATE_H_ */



 1 E F
 2 A B C
 3 D
 4
