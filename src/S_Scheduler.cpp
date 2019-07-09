/*
 * S_Scheduler.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */

#include "S_Scheduler.h"

S_Scheduler::S_Scheduler(string type, int q) : Scheduler(type, q) {

}

void S_Scheduler::add_to_queue(Process *p) {
	list <Process *>::iterator it;
	for (it = runQ.begin() ; it != runQ.end(); it++) {
		if ( (*it)->GetRem() > p->GetRem() ) {	//insert process before process that has longer remaining time
			runQ.insert(it, p);
			return;
		}
	}
	runQ.insert(it, p);
}
Process* S_Scheduler::get_next_process() {
	if (!runQ.empty()) {
		Process * p = runQ.front();
		runQ.pop_front();
		return p;
	} else
		return NULL;
}

