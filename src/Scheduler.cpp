/*
 * Scheduler.cpp
 *
 *  Created on: Jul 6, 2019
 *      Author: sungw
 */

#include "Scheduler.h"


//Scheduler::Scheduler (queue <Process *> *pQ) : allP(pQ) {
//
//};
void Scheduler::push(Process *p) {
	runQ.push(p);
//	cout << "Push: " << p->GetNum() << ' '  <<  runQ.size()  <<  ' ' <<  runQ.front()->GetNum() << endl;
}
Process* Scheduler::pop() {
	Process * p = runQ.front();
	runQ.pop();
//	cout << "Pop: " << p->GetNum() << ' ' <<  runQ.size()  << endl;
	return p;
}
Process* Scheduler::CurrentProcess() {
	if (!runQ.empty()) {
		cout << "Current: " << runQ.size() << ' ' << runQ.front()->GetNum() << endl;
		return runQ.front();
	}
	else
		return NULL;
}

Process* Scheduler::get_next_process() {
//	Process * p;
	if (!runQ.empty()) {
//		p = runQ.front();
//		runQ.pop();
		return pop();
	} else
		return NULL;
}

//Process* Scheduler::Get_New_Process() {
//	Process * p = allP->front();
//	allP->pop();
//	return p;
//}

//Scheduler::~Scheduler() {
//	delete runQ;
//}
