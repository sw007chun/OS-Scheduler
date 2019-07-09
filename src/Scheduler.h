/*
 * Scheduler.h
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include "Process.h"
using namespace std;

class Scheduler {
private:
	string sType;
	int quantum;
public:
	Scheduler (string st, int q);
	virtual ~Scheduler() { };
	virtual void add_to_queue(Process *p) = 0;
	virtual Process* get_next_process() = 0;
	void Print_SType();
	string Get_SType();	//get scheduler type
	int Get_Quantum();

};

#endif /* SCHEDULER_H_ */
