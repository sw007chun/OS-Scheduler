/*
 * P_Scheduler.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */


#include "P_Scheduler.h"

P_Scheduler::P_Scheduler(int q) : Scheduler("PRIO", q) {
	activeQ = new list <Process *>;
	expiredQ = new list <Process *>;
}

void P_Scheduler::add_to_queue(Process *p) {
	list <Process *> *pQ;
	list <Process *> ::iterator it;

	if (p->GetDynPrio() >= 0)
		pQ = activeQ;
	else {
		pQ = expiredQ;
		p->Reset_Prio();
	}

	for (it = (*pQ).begin(); it != (*pQ).end(); it++) {
		if ( (*it)->GetDynPrio() < p->GetDynPrio() ) {
			(*pQ).insert(it, p);
			return;
		}
	}
	(*pQ).insert(it, p);

//	cout << activeQ->empty() <<' ' << pQ->empty() << endl;
}

Process* P_Scheduler::get_next_process() {
//	cout << activeQ->empty() <<' ' << expiredQ->empty() << endl;
	if (!activeQ->empty()) {
		Process * p = activeQ->front();
		activeQ->pop_front();
		return p;
	} else if (!expiredQ->empty())  {
		list <Process *> *temp = activeQ;
		activeQ = expiredQ;
		expiredQ = temp;
		return get_next_process();
	} else
		return NULL;
}
