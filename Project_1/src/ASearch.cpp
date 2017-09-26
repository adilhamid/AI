/*
 * ASearch.cpp
 *
 *  Created on: Sep 21, 2017
 *      Author: adilh
 */

#include "ASearch.h"

ASearch<BlocksWorld>::ASearch() {
	// TODO Auto-generated constructor stub
	currStateVal = STATE_NOTINIT;
	startNode = NULL;
	goalNode = NULL;
	currentNode = NULL;
	countSteps = 0;
	nodeCount = 0;
	depthOfSearch = 0;
}
is
void ASearch<BlocksWorld>::InitASearch(BlocksWorld &startState, BlocksWorld & goalState) {

	startNode = CreateNode();
	goalNode = CreateNode();

	startNode->currentState = startState;
	goalNode->currentState = goalState;

	currStateVal = STATE_SEARCHING;

	//Now Initialize the cost Values of the Nodes accordingly

	startNode->gCost = 0.0;
	startNode->hCost = startNode->currentState.HeuristicsEstimateCost(
			goalNode->currentState);
	startNode->totalCost = startNode->gCost + startNode->hCost;
	startNode->parent = NULL;

	//Now we update the frontier since we have explored the initial or goal node
	frontier.push_back(startNode);
	push_heap(frontier.begin(), frontier.end(), CompareClassNode()); // Heapify the Vector

	//Initialize the step count for how many steps you have taken
	countSteps = 0;
	depthOfSearch = 0;

}

bool ASearch<BlocksWorld>::AddSuccessors(BlocksWorld& presentState) {
	Node * newNode = CreateNode();
	if (!newNode) {
		return false;
	}
	newNode->currentState = presentState;
	successors.push_back(newNode);
	return true;
}

Node* ASearch<BlocksWorld>::CreateNode() {
	Node * newNode = new Node();
	nodeCount++;
	return newNode;
}

void ASearch<BlocksWorld>::deleteNode(Node* node) {
	nodeCount--;
	delete node;
}

void ASearch<BlocksWorld>::deleteProcessedNodes() {
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

void ASearch<BlocksWorld>::deleteUnProcessedNodes() {
}

int ASearch<BlocksWorld>::AsearchExecute() {
	//Initial Conditions needs to be updated

	std::cout << "Here i am " << std::endl;

	// If goal node has been achieved already
	if (currStateVal == STATE_GOAL || currStateVal == STATE_FAIL)
		return currStateVal;

	// Checking if their is any Node left in frontier: if no then  return the goal is not achievable
	if (frontier.empty()) {
		deleteProcessedNodes();
		currStateVal = STATE_FAIL;
		return currStateVal;
	}

	// If frontier is not empty then we add the steps
	countSteps++;

	//Now Get the best Node from the frontier
	Node * nodePop = frontier.front();
	pop_heap(frontier.begin(), frontier.end(), CompareClassNode());
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

		std::vector<BlocksWorld> returnedSuccessors =
				nodePop->currentState.GenerateSuccessors(
						nodePop->parent->currentState);

		//Add the successors to the list
		for (auto val : returnedSuccessors) {
			AddSuccessors(val);
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
			for (; exploredIter != exploredSet.end(); frontierIter++) {
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

				make_heap(frontier.begin(), frontier.end(), CompareClassNode()); // Heapify on invalid heap doesn't work
			}

			frontier.push_back((*successorIter));

			push_heap(frontier.begin(), frontier.end(), CompareClassNode());

		}

		exploredSet.push_back(nodePop);

	}

	return currStateVal;
}

BlocksWorld* ASearch<BlocksWorld>::GetStartSolutionState() {
	currentNode = startNode;
	if (startNode) {
		return &startNode->currentState;
	}
	return NULL;
}

BlocksWorld* ASearch<BlocksWorld>::GetNextSolutionState() {
	if (currentNode) {
		if (currentNode->child) {
			Node *childNode = currentNode->child;
			currentNode = currentNode->child;
			return &childNode->currentState;
		}
	}
	return NULL;
}

BlocksWorld* ASearch<BlocksWorld>::GetSolutionEndState() {
	currentNode = goalNode;
	if (goalNode) {
		return &goalNode->currentState;
	}
	return NULL;

}

BlocksWorld* ASearch<BlocksWorld>::GetSolutionPrevNode() {
	if (currentNode) {
		if (currentNode->parent) {
			Node * parentNode = currentNode->parent;
			currentNode = currentNode->parent;
			return &parentNode->currentState;
		}
	}
	return NULL;
}

void ASearch<BlocksWorld>::TracebackSolution() {
	Node * tempNode = this->goalNode;
	while (tempNode) {
		tempNode->currentState.PrintState();
		tempNode = tempNode->parent;
	}
}

float ASearch<BlocksWorld>::GetTotalSolutionCost() {
	if (goalNode && currStateVal == STATE_GOAL) {
		return goalNode->gCost; // return actual cost to reach the goal == totalCost == gCost + 0
	}
	return -1;
}
