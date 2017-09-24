/*
 * State.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: adilhamidmalla
 */

#include "State.h"
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

void InitializeProblemState(State goalState) {
	assert(numStacks >= 2 && numBlocks >= 1);
	// Putting the Goal State
	for (int counter = 0; counter < numBlocks; ++counter) {
		goalState.stackHolders[0].push_back('A' + counter);
	}
}

int ProblemGenerator(State goalState) {
//	cout << "Inside the Problem Generator Function"<<endl;
//	State initialState(numStacks , numBlocks);
//	InitializeProblemState(initialState) ;
//	initialState.PrintState();
//
//	int numSteps= 0;
//	int randNum = rand()%(maxSteps-minSteps + 1) + minSteps;
//	for (int cnt = 0; cnt < randNum; ++cnt) {
//		int stack
//	}
	return 0;
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

State ProblemGeneratorBinomial() {
	cout << "Initial State generator"<<endl;
	State initialState(numStacks, numBlocks);
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
}

int main(int argc, char* argv[]) {

//	cout << "Please enter number of stacks and number of blocks for the problem"<<endl;
//	cin >> numStacks >> numBlocks;

//	cout << "Initializing the Problem" << endl;
//	State goalState(numStacks, numBlocks);
//	State testState(numStacks, numBlocks);
//	InitializeProblemState(goalState);
//	InitializeProblemState(testState);
//	testState.stackHolders[1].push_back(testState.stackHolders[0].back());
//	testState.stackHolders[0].pop_back();
//	goalState.PrintState();
//	testState.PrintState();
//	cout << "IsSameState " << testState.IsGoalNode(goalState) << endl;
//	cout << "IsSameState " << goalState.IsGoalNode(testState) << endl;
//
//	cout << "H1 Heuristics " << testState.HeuristicsEstimateCost(goalState)
//			<< endl;

	ProblemGeneratorBinomial();

	cout << "I am running this problem now man" << endl;

	return 0;

}

State::State() {
// TODO Auto-generated constructor stub
}

State::State(int pNumStacks, int pNumBlocks) {
	stackHolders = new std::vector<char>[pNumStacks];
	numBlocks = pNumBlocks;
	numStacks = pNumStacks;
}

bool State::IsGoalNode(State goalState) {

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

std::vector<State> State::GenerateSuccessors(State currentState) {
	vector<State> returnVal;

	return returnVal;
}

bool State::IsIdenticalState(State currentState) {

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

float State::GetGCost(State currentState) {
// Since the cost to each of the step is just one so the Cost is uniform for now
	return 1.0;
}

float State::HeuristicsEstimateCost(State goalState) {

// Use Different Heuristics for solving the problem
	float h1 = HeuristicsOneCost(goalState);
	return h1;
}

void State::PrintState() {
	for (int stackCnt = 0; stackCnt < this->numStacks; stackCnt++) {
		cout << "Stack " << stackCnt << ": ";
		for (auto val : this->stackHolders[stackCnt]) {
			cout << val << " ";
		}
		cout << endl;
	}
}
// get the cost of misplaced blocks on the stacks
float State::HeuristicsOneCost(State goalState) {
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

State::~State() {
// TODO Auto-generated destructor stub
}

