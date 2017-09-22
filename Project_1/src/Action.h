/*
 * Action.h
 *
 *  Created on: Sep 20, 2017
 *      Author: adilhamidmalla
 */

#ifndef ACTION_H_
#define ACTION_H_

class Action {
private:
	int fromStackNum;
	int toStackNum;
	char charBlockName;
public:
	Action();
	virtual ~Action();
};

#endif /* ACTION_H_ */
