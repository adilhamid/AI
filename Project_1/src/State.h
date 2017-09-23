/*
 * State.h
 *
 *  Created on: Sep 20, 2017
 *      Author: adilhamidmalla
 */

#ifndef STATE_H_
#define STATE_H_

#include <stack>
#include <vector>

class State {
private:
	std::stack<char> *stackHolders;
	int numOfHolders;

public:
	State();
	State(int);
	bool IsGoalNode(State currentState);
	std::vector<State> GenerateSuccessors(State currentState);
	bool IsIdenticalState(State currentState);
	float GetGCost(State currentState);
	float HeuristicsEstimateCost(State currentState);
	~State();
};

#endif /* STATE_H_ */
