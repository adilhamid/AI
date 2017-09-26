/*
 * Node.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: adilhamidmalla
 */

#include "Node.h"

Node::Node() {
	parent = NULL;
	child = NULL;
	gCost = 0.0;
	hCost = 0.0;
	totalCost = 0.0;

}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

