/*
 * State.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: adilhamidmalla
 */

#include "BlocksWorldProblem.h"

#include "ASearch.h"
#include <iostream>
#include <vector>
#include <assert.h>
#include <ctime>

#define MASS_RUN
#undef MASS_RUN

using namespace std;

//Global Variables
int numStacks = 0;
int numBlocks = 0;

BlocksWorldProblem SetGoalState() {
	BlocksWorldProblem goalState(numStacks, numBlocks);
	assert(numStacks >= 2 && numBlocks >= 1);
	// Putting the Goal State
	for (int counter = 0; counter < numBlocks; ++counter) {
		goalState.stackHolders[0].push_back('A' + counter);
	}
	return goalState;
}

void swap(char *a, char *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
// A function to generate a random permutation of arr[]
void randomize(vector<char>& arr, int n) {
	srand(time(NULL));
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(arr[i], arr[j]);
	}
}

BlocksWorldProblem ProblemGenerator() {

	BlocksWorldProblem initialState(numStacks, numBlocks);

//	initialState.stackHolders[0].push_back('D');
//	initialState.stackHolders[1].push_back('E');
//	initialState.stackHolders[1].push_back('F');
//	initialState.stackHolders[1].push_back('I');
//	initialState.stackHolders[1].push_back('J');
//
//	initialState.stackHolders[2].push_back('B');
//	initialState.stackHolders[2].push_back('G');
//
//	initialState.stackHolders[3].push_back('C');
//	initialState.stackHolders[3].push_back('H');
//
//	initialState.stackHolders[4].push_back('A');
//
//	return initialState;

	int totalStacks = numStacks;
	vector<char> arrayList(numBlocks);
	for (int cnt = 0; cnt < numBlocks; ++cnt) {
		arrayList[cnt] = 'A' + cnt;
	}
	randomize(arrayList, numBlocks);
	totalStacks = numBlocks;
	// Multinomial Distribution of elements in
	for (int cntStack = 0; cntStack < numStacks; ++cntStack) {
		if (cntStack != numStacks - 1) {
			int randStack = rand() % (totalStacks + 1);
			for (int cntBlock = 0; cntBlock < randStack; ++cntBlock) {
				initialState.stackHolders[cntStack].push_back(arrayList.back());
				arrayList.pop_back();
			}
			totalStacks -= randStack;
		} else {
			for (int cntBlock = 0; cntBlock < totalStacks; ++cntBlock) {
				initialState.stackHolders[cntStack].push_back(arrayList.back());
				arrayList.pop_back();
			}

		}

	}
#ifndef MASS_RUN
	cout << "Initial State " << endl;
	initialState.PrintState();
#endif

	return initialState;
}

BlocksWorldProblem::BlocksWorldProblem() {
// TODO Auto-generated constructor stub
}

BlocksWorldProblem::BlocksWorldProblem(int pNumStacks, int pNumBlocks) {
	stackHolders = new std::vector<char>[pNumStacks];
	numBlocks = pNumBlocks;
	numStacks = pNumStacks;
}

bool BlocksWorldProblem::IsGoalNode(BlocksWorldProblem goalState) {

	for (int cntStack = 0; cntStack < this->numStacks; ++cntStack) {
		if (this->stackHolders[cntStack].size()
				!= goalState.stackHolders[cntStack].size()) {
			return false;
		}
		for (int cntBlocks = 0;
				cntBlocks < goalState.stackHolders[cntStack].size();
				++cntBlocks) {
			if (this->stackHolders[cntStack][cntBlocks]
					!= goalState.stackHolders[cntStack][cntBlocks]) {
				return false;
			}
		}
	}
	return true;
}
void DeepCopyState(BlocksWorldProblem sourceState,
		BlocksWorldProblem destState) {
	for (int cntStack = 0; cntStack < numStacks; ++cntStack) {
		for (int cntBlocks = 0;
				cntBlocks < sourceState.stackHolders[cntStack].size();
				++cntBlocks) {
			destState.stackHolders[cntStack].push_back(
					sourceState.stackHolders[cntStack][cntBlocks]);
		}
	}
	return;
}
std::vector<BlocksWorldProblem> BlocksWorldProblem::GenerateSuccessors(
		BlocksWorldProblem currentState) {

	vector<BlocksWorldProblem> returnVal;
	for (int cntStack = 0; cntStack < numStacks; ++cntStack) {
		if (currentState.stackHolders[cntStack].size() > 0) {
			char topElement = currentState.stackHolders[cntStack].back();
			for (int innerCntStack = 0; innerCntStack < numStacks;
					++innerCntStack) {
				if (cntStack == innerCntStack)
					continue;

				// Add the State to the list of successors which is accessible from the current node.
				BlocksWorldProblem tempState(numStacks, numBlocks);
				//DeepCopy the State
				DeepCopyState(currentState, tempState);

				tempState.stackHolders[cntStack].pop_back();
				tempState.stackHolders[innerCntStack].push_back(topElement);

//				tempState.PrintState();
				returnVal.push_back(tempState);

			}
		}
	}
	return returnVal;
}

bool BlocksWorldProblem::IsIdenticalState(BlocksWorldProblem currentState) {

	for (int cntStack = 0; cntStack < this->numStacks; ++cntStack) {
		if (this->stackHolders[cntStack].size()
				!= currentState.stackHolders[cntStack].size()) {
			return false;
		}
		for (int cntBlocks = 0;
				cntBlocks < currentState.stackHolders[cntStack].size();
				++cntBlocks) {
			if (this->stackHolders[cntStack][cntBlocks]
					!= currentState.stackHolders[cntStack][cntBlocks]) {
				return false;
			}
		}
	}
	return true;
}

float BlocksWorldProblem::GetGCost(BlocksWorldProblem currentState) {
// Since the cost to each of the step is just one so the Cost is uniform for now
	return 1.0;
}

float BlocksWorldProblem::HeuristicsEstimateCost(BlocksWorldProblem goalState) {

// Use Different Heuristics for solving the problem

	float totalHCost = 0;
	float w1 = 2; // Weight for the h1 heuristics

	float h1 = this->HeuristicsOneCost();
	float h3 = HeuristicsThreeCost(goalState);
	totalHCost = w1 * h1 + h3;
	return totalHCost;
}

void BlocksWorldProblem::PrintState() {
	for (int stackCnt = 0; stackCnt < this->numStacks; stackCnt++) {
		cout << "Stack " << stackCnt << ": ";
		for (auto val : this->stackHolders[stackCnt]) {
			cout << val << " ";
		}
		cout << endl;
	}
	cout << endl;
}
// get the cost of misplaced blocks on the stacks
float BlocksWorldProblem::HeuristicsOneCost() {
	int h1Val = 0;
// Number of elements present in the First Stack + misplaced stacks
	h1Val += numBlocks - this->stackHolders[0].size();

	for (int cntBlocks = 0; cntBlocks < this->stackHolders[0].size();
			++cntBlocks) {
		if (this->stackHolders[0][cntBlocks] == 'A' + cntBlocks) {
			continue;
		} else {
			h1Val += this->stackHolders[0].size() - cntBlocks;
			return h1Val;
		}
	}
	return h1Val;
}

float BlocksWorldProblem::HeuristicsTwoCost(
		BlocksWorldProblem blocksWorldProblem) {
	int h2Cost = 0;
	// Check the goal stack value
	for (int i = 1; i < blocksWorldProblem.stackHolders[0].size(); i++) {
		if (blocksWorldProblem.stackHolders[0][i]
				!= blocksWorldProblem.stackHolders[0][i - 1] + 1) {
			h2Cost += 2;
		}
	}

	// For other stacks
	for (int cntStack = 1; cntStack < numStacks; cntStack++) {
		for (int cntBlock = 1;
				cntBlock < blocksWorldProblem.stackHolders[cntStack].size();
				cntBlock++) {
			if (blocksWorldProblem.stackHolders[cntStack][cntBlock]
					!= blocksWorldProblem.stackHolders[cntStack][cntBlock - 1]
							+ 1) {
				h2Cost += 2;
			}
		}
	}
	return h2Cost;
}

float BlocksWorldProblem::HeuristicsThreeCost(
		BlocksWorldProblem blocksWorldProblem) {
	int h3Cost = 0;

	int numBlocksOnce = 0;
	int numBlocksTwice = 0;

	bool movFlag = 0;

	for (int i = 1; i < blocksWorldProblem.stackHolders[0].size(); i++) {

		if (blocksWorldProblem.stackHolders[0][i - 1]
				!= blocksWorldProblem.stackHolders[0][i] - 1) {
			movFlag = 1;
			if (i == 1) {
				numBlocksOnce += 1;
			}
			numBlocksOnce += 1;
		} else {
			if (movFlag) {
				numBlocksTwice += 1;
			}
		}
	}

	// For other stacks
	for (int cntStack = 1; cntStack < numStacks; cntStack++) {
		movFlag = false;
		for (int i = 1; i < blocksWorldProblem.stackHolders[cntStack].size();
				i++) {
			if (blocksWorldProblem.stackHolders[cntStack][i - 1]
					!= blocksWorldProblem.stackHolders[cntStack][i] + 1) {
				movFlag = 1;
				if (i == 1) {
					numBlocksOnce += 1;
				}
				numBlocksOnce += 1;
			} else {
				if (movFlag) {
					numBlocksTwice += 1;
				}
			}
		}
	}
	h3Cost += (2 * numBlocksOnce + 4 * numBlocksTwice);
	return h3Cost;
}

BlocksWorldProblem::~BlocksWorldProblem() {
// TODO Auto-generated destructor stub
}

int main(int argc, char* argv[]) {

#ifdef MASS_RUN // Mass run for calculating the averages
	int numIterations = 100;
	float averageGoalTests, avgMaximumQueueSize, avgSolutionPathLength;

	for (int stacksCnt = 10; stacksCnt <= 10; ++stacksCnt) {

		numStacks = stacksCnt;

		for (int blocksCnt = 5; blocksCnt <= 10; ++blocksCnt) {
			numBlocks = blocksCnt;
			averageGoalTests = avgMaximumQueueSize = avgSolutionPathLength =
					0.0;

			for (int iter = 0; iter < numIterations; ++iter) {

				// Starting the initial state and goal state
				BlocksWorldProblem initialState = ProblemGenerator();

				BlocksWorldProblem goalState = SetGoalState();

				// Initialize the blocksworld A* Search
				ASearch<BlocksWorldProblem> blocksworld;

				blocksworld.InitProblemState(initialState, goalState);

				int totalGoalTests = 0;
				ASearcReturnVal resultVal;

				//Searching algorithm starts
				do {
					resultVal = blocksworld.ASearchExecute();
					totalGoalTests++;

				} while (resultVal.resultState
						== ASearch<BlocksWorldProblem>::STATE_SEARCHING);

				if (resultVal.resultState
						== ASearch<BlocksWorldProblem>::STATE_GOAL) {
					averageGoalTests += totalGoalTests;
					avgMaximumQueueSize += resultVal.maxQueueSize;
					avgSolutionPathLength += blocksworld.GetTotalSolutionCost();

				} else {
					cout << "Could not found the goal due to time out" << endl;
				}

				blocksworld.deleteProcessedNodes();

			}

			// Calculate the averages and print for record keeping
			averageGoalTests /= numIterations;
			avgMaximumQueueSize /= numIterations;
			avgSolutionPathLength /= numIterations;

			cout << stacksCnt << " " << blocksCnt << " " << averageGoalTests
					<< " " << avgMaximumQueueSize << " "
					<< avgSolutionPathLength << endl;

		}

	}

#else

	cout << "Stacks and Blocks please" << endl;

	cin >> numStacks >> numBlocks;

	// Starting the initial state and goal state
	BlocksWorldProblem initialState = ProblemGenerator();

	BlocksWorldProblem goalState = SetGoalState();

	// Initialize the blocksworld A* Search
	ASearch<BlocksWorldProblem> blocksworld;

	blocksworld.InitProblemState(initialState, goalState);

	int totalGoalTests = 0;
	ASearcReturnVal resultVal;

	//Searching algorithm starts
	do {
		resultVal = blocksworld.ASearchExecute();
		totalGoalTests++;

	}while (resultVal.resultState
			== ASearch<BlocksWorldProblem>::STATE_SEARCHING);

	if (resultVal.resultState
			== ASearch<BlocksWorldProblem>::STATE_GOAL) {
		cout << "Goal State Found " << endl;
		cout << "Success!! depth = " << resultVal.depthOfGoalState
		<< ", Total Goal Tests = " << totalGoalTests
		<< ", Maximum Queue Size= "
		<< resultVal.maxQueueSize << ", Total Cost = "
		<< blocksworld.GetTotalSolutionCost() << endl;
		blocksworld.TracebackSolution();
	} else {
		cout << "Could not found the goal due to time out" << endl;
	}
	cout << "Done and Nailed It " << endl;

#endif

	return 0;
}
