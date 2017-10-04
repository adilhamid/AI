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
#include <unordered_set>
#include <string>

#include "BlocksWorldProblem.h"
#include "Node.h"
#include "NodeComparison.h"

using namespace std;

struct ASearcReturnVal {
	int resultState;
	int depthOfGoalState;
	int maxQueueSize;
};

template<class ProblemState> class ASearch {

public:

	enum {
		STATE_INVALID, STATE_SEARCHING, STATE_GOAL, STATE_NOTINIT, STATE_FAIL

	};

	int countSteps;
	int currStateVal;
	int nodeCount;
	int maxQueueSize;

	Node * startNode;
	Node * goalNode;
	Node * currentNode;
	std::vector<Node*> frontier;

	std::vector<Node *> successors;
	//std::vector<Node *> exploredSet;

	//New Explored Set Functionality
	std::unordered_set<string> exploredStateSet;

	ASearcReturnVal returnAsearchVal;

	ASearch() {
		currStateVal = STATE_NOTINIT;
		startNode = NULL;
		goalNode = NULL;
		currentNode = NULL;
		countSteps = 0;
		nodeCount = 0;
		maxQueueSize = INT_MIN;
	}

//	~ASearch(){
//		if(startNode){
//			deleteNode(startNode);
//		}
//		if(goalNode){
//			deleteNode(goalNode);
//		}
//		if(currentNode){
//			deleteNode(currentNode);
//		}
//	}

// Member Functions
	void InitProblemState(ProblemState& startState, ProblemState& goalState);
	bool AddSuccessor(ProblemState &presentState);
	bool AddSuccessorsInBulk(vector<BlocksWorldProblem>, Node*);

	Node * CreateNode();
	void deleteNode(Node * node);
	void deleteProcessedNodes();
	void deleteUnProcessedNodes();
	string getHashCode(BlocksWorldProblem);

	ASearcReturnVal ASearchExecute();
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
inline bool ASearch<ProblemState>::AddSuccessorsInBulk(
		vector<BlocksWorldProblem> allocator, Node* parentNode) {

	for (auto val : allocator) {

		Node * newNode = CreateNode();
		if (!newNode) {
			return false;
		}
		newNode->currentState = val;
		newNode->depth = parentNode->depth + 1;
		successors.push_back(newNode);
		return true;

	}

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

//	Delete the closeList of Nodes
	exploredStateSet.clear();

	//Delete the goal Node
	deleteNode(goalNode);

	//Delete Current Node
	deleteNode(currentNode);
}

template<class ProblemState>
inline void ASearch<ProblemState>::deleteUnProcessedNodes() {
}

template<class ProblemState>
inline string ASearch<ProblemState>::getHashCode(
		BlocksWorldProblem blocksWorldProblem) {

	string result = "";
	for (int i = 0; i < numStacks; i++) {
		for (int j = 0; j < blocksWorldProblem.stackHolders[i].size(); j++) {
			result += blocksWorldProblem.stackHolders[i][j];
		}
		result += ",";
	}
	return result;
}

template<class ProblemState>
inline ASearcReturnVal ASearch<ProblemState>::ASearchExecute() {
	//Initial Conditions needs to be updated

	if (currStateVal == STATE_GOAL || currStateVal == STATE_FAIL) {
		return returnAsearchVal;
	}

	if (frontier.empty()) {
		deleteProcessedNodes();
		currStateVal = STATE_FAIL;
		returnAsearchVal.resultState = currStateVal;
		return returnAsearchVal;
	}

	Node * nodePop = frontier.front();
	pop_heap(frontier.begin(), frontier.end(), NodeComparison());
	frontier.pop_back();

	// If frontier is not empty then we add the steps
	countSteps++;

	// Goal test the node popped from frontier
	if (nodePop->currentState.IsGoalNode(goalNode->currentState)) {
		goalNode->parent = nodePop->parent;
		goalNode->totalCost = nodePop->totalCost;
		goalNode->gCost = nodePop->gCost;
		goalNode->hCost = nodePop->hCost;
		goalNode->depth = nodePop->depth;

		// Special Case of whether the goal present is start goal
		// Where to handle the same ??? No Idea

		currStateVal = STATE_GOAL;

	} else {  // Not Goal State then explore the more branches of the node

		successors.clear();

		std::vector<BlocksWorldProblem> returnedSuccessors =
				nodePop->currentState.GenerateSuccessors(nodePop->currentState);

		//Add the successors to the list
		for (auto val : returnedSuccessors) {
			AddSuccessor(val);
		}

		if (successors.empty()) { // If the successors were not generated due to memory issue or some other issue

			cout << "Error Memory Issue or something else is happening" << endl;

			for (auto iter = successors.begin(); iter != successors.end();
					iter++) {
				deleteNode(*iter);
			}

			successors.clear();

			currStateVal = STATE_FAIL;
			std::cout << "Ran out of Memory : Successor Fault" << std::endl;

		} else {

			// Handling the case where the node has successors

			for (auto successorIter = successors.begin();
					successorIter != successors.end(); successorIter++) {

				float updatedGCost = nodePop->gCost
						+ nodePop->currentState.GetGCost(
								(*successorIter)->currentState); // Get Cost will be user dependent

						// Checking the frontier-- if Goal Node exists in the frontier and the cost in frontier is less than that of successor then we delete the successor
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
//				auto exploredIter = exploredSet.begin();
//				for (; exploredIter != exploredSet.end(); exploredIter++) {
//					if ((*exploredIter)->currentState.IsIdenticalState(
//							(*successorIter)->currentState)) {
//						break;
//					}
//				}
//				if (exploredIter != exploredSet.end()) {
//					if ((*exploredIter)->gCost <= updatedGCost) {
//						deleteNode(*successorIter);
//						continue;
//					}
//				}

//				 Enhanced way of comparing the results
				if (exploredStateSet.find(
						getHashCode((*successorIter)->currentState))
						!= exploredStateSet.end()) {
					deleteNode(*successorIter);
					continue;
				}

				// End of Checking the already present states

				// updating the properties of the successors nodes
				(*successorIter)->parent = nodePop;
				(*successorIter)->gCost = updatedGCost;
				(*successorIter)->hCost =
						(*successorIter)->currentState.HeuristicsEstimateCost(
								(*successorIter)->currentState);
				(*successorIter)->totalCost = (*successorIter)->gCost
						+ (*successorIter)->hCost;
				(*successorIter)->depth = nodePop->depth + 1;

				// Remove from explored Node as it is visited again and updated
//				if (exploredIter != exploredSet.end()) {
//					cout << "I am weird and i go here as well " << endl;
//					deleteNode((*exploredIter));
//					exploredSet.erase(exploredIter);
//				}

				// If the node is present in the frontier also then we need to update the same
				if (frontierIter != frontier.end()) {
					deleteNode((*frontierIter));
					frontier.erase(frontierIter);

					make_heap(frontier.begin(), frontier.end(),
							NodeComparison()); // Heapify on invalid heap doesn't work

				}
				frontier.push_back((*successorIter));
				push_heap(frontier.begin(), frontier.end(), NodeComparison());

			}

//			exploredSet.push_back(nodePop);
			exploredStateSet.insert(getHashCode(nodePop->currentState));

			if ((int) frontier.size() > maxQueueSize) {
				maxQueueSize = frontier.size();
			}

		}
	}

//Write the Iterations Details about the explored Node
#if 1
	cout << "Iteration Number " << countSteps << ", Queue Size = "
			<< frontier.size() << ", TotalCost(gCost+hCost) = "
			<< nodePop->totalCost << ", Depth= " << nodePop->depth << endl;
#endif

	returnAsearchVal.depthOfGoalState = nodePop->depth;
	returnAsearchVal.resultState = currStateVal;
	returnAsearchVal.maxQueueSize = maxQueueSize;
	return returnAsearchVal;
}

template<class ProblemState>
inline float ASearch<ProblemState>::GetTotalSolutionCost() {

	if (goalNode && currStateVal == STATE_GOAL) {
		return (goalNode->gCost + goalNode->hCost); // return actual cost to reach the goal == totalCost == gCost + 0
	}
	return -1;

}

template<class ProblemState>
inline void ASearch<ProblemState>::TracebackSolution() {
	Node * tempNode = goalNode;
	stack<Node*> printSolution;
	while (tempNode) {
		printSolution.push(tempNode);
		tempNode = tempNode->parent;
	}

	while (!printSolution.empty()) {
		printSolution.top()->currentState.PrintState();
		printSolution.pop();
	}
}

#endif /* ASEARCH_H_ */

