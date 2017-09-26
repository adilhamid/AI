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

using namespace std;

//Global Variables
int numStacks = 3;
int numBlocks = 5;
int maxSteps = 100;
int minSteps = 0;

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
void randomize(vector<char> arr, int n) {
	srand(time(NULL));
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(arr[i], arr[j]);
	}
}

BlocksWorldProblem ProblemGenerator() {
	cout << "Initial State generator..." << endl;
	BlocksWorldProblem initialState(numStacks, numBlocks);
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
	initialState.PrintState();
	cout << "Ending the Problem generator....." << endl;
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
void DeepCopyState(BlocksWorldProblem sourceState, BlocksWorldProblem destState) {
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
std::vector<BlocksWorldProblem> BlocksWorldProblem::GenerateSuccessors(BlocksWorldProblem currentState) {

	cout << "Generating the Successors" << endl;
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

				tempState.PrintState();
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
	float h1 = HeuristicsOneCost(goalState);
	return h1;
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
float BlocksWorldProblem::HeuristicsOneCost(BlocksWorldProblem goalState) {
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

BlocksWorldProblem::~BlocksWorldProblem() {
// TODO Auto-generated destructor stub
}

int main(int argc, char* argv[]) {

	// Starting the initial state and goal state
	BlocksWorldProblem initialState = ProblemGenerator();
	BlocksWorldProblem goalState = SetGoalState();

	// Initialize the blocksworld A* Search
	ASearch<BlocksWorldProblem> blocksworld;

	blocksworld.InitProblemState(initialState, goalState);

	int resultState;
	int searchSteps = 0;

	//Searching algorithm starts
	do {
		resultState = blocksworld.ASearchExecute();
		cout << "Result State " << endl;
		cout << "Search Step number: " << searchSteps << endl;
		searchSteps++;

	} while (resultState == ASearch<BlocksWorldProblem>::STATE_SEARCHING);

	if (resultState == ASearch<BlocksWorldProblem>::STATE_GOAL) {
		cout << "Goal State Found " << endl;
		cout << "Total Cost " << blocksworld.GetTotalSolutionCost()<<endl;
		blocksworld.TracebackSolution();

	}
	cout << "Done and dusted " << endl;
	return 0;

}