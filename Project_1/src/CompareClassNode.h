/*
 * CompareClass.h
 *
 *  Created on: Sep 21, 2017
 *      Author: adilhamidmalla
 */

#ifndef COMPARECLASSNODE_H_
#define COMPARECLASSNODE_H_

class CompareClassNode {
public:
	bool operator() ( const Node *x, const Node *y ) const
				{
					return x->totalCost > y->totalCost;
				}
};

#endif /* COMPARECLASSNODE_H_ */
