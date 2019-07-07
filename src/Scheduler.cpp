/*
 * Scheduler.cpp
 *
 *  Created on: Jul 6, 2019
 *      Author: sungw
 */

#include "Scheduler.h"


Scheduler::Scheduler (queue <Process *> *pQ) : allP(pQ) {

};
void Scheduler::push(Process *p) {
	runQ.push(p);
}
Process* Scheduler::pop() {
	Process * p = runQ.front();
	runQ.pop();
	return p;
}
Process* Scheduler::CurrentProcess() {
//	cout << runQ.size() << endl;
	if (!runQ.empty())
		return runQ.front();
	else
		return NULL;
}

Process* Scheduler::get_next_process() {
	Process * p = allP->front();
	allP->pop();
	return p;
}

int Scheduler::Get_Next_Process_Arrival() {
	return allP->front()->GetTimeStamp();
}

//Scheduler::~Scheduler() {
//	delete runQ;
//}
