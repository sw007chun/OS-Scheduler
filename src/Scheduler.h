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
	Process* CurrentProcess();
	virtual void add_to_queue(Process *p) = 0;
	Process* get_next_process();
	int Get_Next_Process_Arrival();

};

#endif /* SCHEDULER_H_ */
