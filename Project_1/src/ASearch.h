/*
 * ASearch.h
 *
 *  Created on: Sep 21, 2017
 *      Author: adilhamidmalla
 */

#ifndef ASEARCH_H_
#define ASEARCH_H_

#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>

#include "BlocksWorldProblem.h"
#include "Node.h"
#include "NodeComparison.h"

using namespace std;

template<class ProblemState> class ASearch {

public:

	enum {
		STATE_INVALID, STATE_SEARCHING, STATE_GOAL, STATE_NOTINIT, STATE_FAIL

	};

	int countSteps;
	int currStateVal;
	int depthOfSearch;
	int nodeCount;

	Node * startNode;
	Node * goalNode;
	Node * currentNode;
	std::vector<Node*> frontier;

	std::vector<Node *> successors;
	std::vector<Node *> exploredSet;

// Constructor
	ASearch() {
		currStateVal = STATE_NOTINIT;
		startNode = NULL;
		goalNode = NULL;
		currentNode = NULL;
		countSteps = 0;
		nodeCount = 0;
		depthOfSearch = 0;
	}

	// Member Functions
	void InitProblemState(ProblemState& startState, ProblemState& goalState);
	bool AddSuccessor(ProblemState &presentState);

	Node * CreateNode();
	void deleteNode(Node * node);
	void deleteProcessedNodes();
	void deleteUnProcessedNodes();

	int ASearchExecute();
	float GetTotalSolutionCost();
	void TracebackSolution();

}
;

template<class ProblemState>
inline void ASearch<ProblemState>::InitProblemState(ProblemState& startState,
		ProblemState& goalState) {
	startNode = CreateNode();
	goalNode = CreateNode();

	startNode->currentState = startState;
	goalNode->currentState = goalState;

	currStateVal = STATE_SEARCHING;

	startNode->gCost = 0.0;
	startNode->hCost = startNode->currentState.HeuristicsEstimateCost(
			goalNode->currentState);
	startNode->totalCost = startNode->gCost + startNode->hCost;
	startNode->parent = NULL;

	frontier.push_back(startNode);
	push_heap(frontier.begin(), frontier.end(), NodeComparison()); // Heapify the Vector

	countSteps = 0;
	depthOfSearch = 0;

}

template<class ProblemState>
inline bool ASearch<ProblemState>::AddSuccessor(ProblemState& presentState) {

	Node * newNode = CreateNode();
	if (!newNode) {
		return false;
	}
	newNode->currentState = presentState;
	successors.push_back(newNode);
	return true;

}

template<class ProblemState>
inline Node* ASearch<ProblemState>::CreateNode() {
	Node * newNode = new Node();
	nodeCount++;
	return newNode;

}

template<class ProblemState>
inline void ASearch<ProblemState>::deleteNode(Node* node) {
	nodeCount--;
	delete node;
}

template<class ProblemState>
inline void ASearch<ProblemState>::deleteProcessedNodes() {
	//Delete frontier
	for (auto val : frontier) {
		Node * temp = val;
		deleteNode(temp);
	}
	frontier.clear();
	//Delete the closeList of Nodes
	for (auto val : exploredSet) {
		Node * temp = val;
		deleteNode(temp);
	}
	exploredSet.clear();
	//Delete the goal Node
	deleteNode(goalNode);
}

template<class ProblemState>
inline void ASearch<ProblemState>::deleteUnProcessedNodes() {
}

template<class ProblemState>
inline int ASearch<ProblemState>::ASearchExecute() {
	//Initial Conditions needs to be updated
	std::cout << "Here i am " << std::endl;

	if (currStateVal == STATE_GOAL || currStateVal == STATE_FAIL)
		return currStateVal;

	if (frontier.empty()) {
		deleteProcessedNodes();
		currStateVal = STATE_FAIL;
		return currStateVal;
	}

	// If frontier is not empty then we add the steps
	countSteps++;

	//Now Get the best Node from the frontier
	Node * nodePop = frontier.front();
	pop_heap(frontier.begin(), frontier.end(), NodeComparison());
	frontier.pop_back();

	// Goal test the node popped from frontier
	if (nodePop->currentState.IsGoalNode(goalNode->currentState)) {
		goalNode->parent = nodePop->parent;
		goalNode->totalCost = nodePop->totalCost;
		goalNode->gCost = nodePop->gCost;
		goalNode->hCost = nodePop->hCost;

		// Special Case of whether the goal present is start goal
		// Where to handle the same ??? No Idea

		currStateVal = STATE_GOAL;
		return currStateVal;

	} else {  // Not Goal State then explore the more branches of the node

		successors.clear();

//			bool returnVal = nodePop->currentState.GenerateSuccessor(successors,
//					nodePop ? &nodePop->Parent->currentState : NULL);

		std::vector<BlocksWorldProblem> returnedSuccessors =
				nodePop->currentState.GenerateSuccessors(nodePop->currentState);

		//Add the successors to the list
		for (auto val : returnedSuccessors) {
			AddSuccessor(val);
		}

		if (successors.empty()) { // If the successors were not generated due to memory issue or some other issue

			std::cout << "Am i doing it right or wrong" << std::endl;

			for (auto iter = successors.begin(); iter != successors.end();
					iter++) {
				deleteNode(*iter);
			}

			successors.clear();

			currStateVal = STATE_FAIL;
			std::cout << "Ran out of Memory : Successor Fault" << std::endl;
			return currStateVal;

		}

		// Handling the case where the node has successors

		for (auto successorIter = successors.begin();
				successorIter != successors.end(); successorIter++) {

			float updatedGCost = nodePop->gCost
					+ nodePop->currentState.GetGCost(
							(*successorIter)->currentState); // Get Cost will be user dependent

					// Checking the frontier
			auto frontierIter = frontier.begin();
			for (; frontierIter != frontier.end(); frontierIter++) {
				if ((*frontierIter)->currentState.IsIdenticalState(
						(*successorIter)->currentState)) {
					break;
				}
			}
			if (frontierIter != frontier.end()) {
				if ((*frontierIter)->gCost <= updatedGCost) {
					deleteNode(*successorIter);
					continue;
				}
			}

			//Checking the Explored Nodes
			auto exploredIter = exploredSet.begin();
			for (; exploredIter != exploredSet.end(); exploredIter++) {
				if ((*exploredIter)->currentState.IsIdenticalState(
						(*successorIter)->currentState)) {
					break;
				}
			}
			if (exploredIter != exploredSet.end()) {
				if ((*exploredIter)->gCost <= updatedGCost) {
					deleteNode(*successorIter);
					continue;
				}
			}
			// End of Checking the already present states

			(*successorIter)->parent = nodePop;
			(*successorIter)->gCost = updatedGCost;
			(*successorIter)->hCost =
					(*successorIter)->currentState.HeuristicsEstimateCost(
							goalNode->currentState);
			(*successorIter)->totalCost = (*successorIter)->gCost
					+ (*successorIter)->hCost;

			// Remove from explored Node as it is visited again and updated
			if (exploredIter != exploredSet.end()) {
				deleteNode((*exploredIter));
				exploredSet.erase(exploredIter);
			}

			// If the node is present in the frontier also then we need to update the same
			if (frontierIter != frontier.end()) {
				deleteNode((*frontierIter));
				frontier.erase(frontierIter);

				make_heap(frontier.begin(), frontier.end(), NodeComparison()); // Heapify on invalid heap doesn't work
			}

			frontier.push_back((*successorIter));

			push_heap(frontier.begin(), frontier.end(), NodeComparison());

		}

		exploredSet.push_back(nodePop);

	}

	return currStateVal;
}

template<class ProblemState>
inline float ASearch<ProblemState>::GetTotalSolutionCost() {

	if (goalNode && currStateVal == STATE_GOAL) {
		return goalNode->gCost; // return actual cost to reach the goal == totalCost == gCost + 0
	}
	return -1;

}

template<class ProblemState>
inline void ASearch<ProblemState>::TracebackSolution() {
	Node * tempNode = goalNode;
	while (tempNode) {
		tempNode->currentState.PrintState();
		tempNode = tempNode->parent;
	}
}

#endif /* ASEARCH_H_ */

