/*
 * Scheduler.h
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <iostream>
#include <queue>
#include "Process.h"
using namespace std;

class Scheduler {
private:
	queue <Process *> *allP;
	queue <Process *> runQ;
public:
	Scheduler (queue <Process *> *pQ);
	void push(Process *p);
	Process* pop();
//	virtual ~Scheduler();
	virtual void add_to_queue(Process *p) = 0;
	virtual Process* get_next_process() = 0;
};

#endif /* SCHEDULER_H_ */
