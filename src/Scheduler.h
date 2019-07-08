/*
 * Scheduler.h
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <iostream>
#include "Process.h"
using namespace std;

class Scheduler {
private:
//	queue <Process *> runQ;
	string sType;
public:
	Scheduler (string st);
//	void push(Process *p);
//	Process* pop();
	virtual ~Scheduler() { };
	virtual void add_to_queue(Process *p) = 0;
	virtual Process* get_next_process() = 0;
	string Get_SType();

};

#endif /* SCHEDULER_H_ */
