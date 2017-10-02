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

class BlocksWorldProblem {

public:
	float HeuristicsOneCost();
	float HeuristicsTwoCost(BlocksWorldProblem);
	float HeuristicsThreeCost(BlocksWorldProblem);

public:
	//This represents the state of the problem
	std::vector<char> *stackHolders;
	int numStacks;
	int numBlocks;

public:
	BlocksWorldProblem();
	BlocksWorldProblem(int, int);
	bool IsGoalNode(BlocksWorldProblem );
	std::vector<BlocksWorldProblem> GenerateSuccessors(BlocksWorldProblem );
	bool IsIdenticalState(BlocksWorldProblem );
	float GetGCost(BlocksWorldProblem );
	float HeuristicsEstimateCost(BlocksWorldProblem );
	void PrintState();
	~BlocksWorldProblem();


};

#endif /* STATE_H_ */
