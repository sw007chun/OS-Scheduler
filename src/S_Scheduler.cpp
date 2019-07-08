/*
 * S_Scheduler.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */

#include "S_Scheduler.h"

S_Scheduler::S_Scheduler(int q) : Scheduler("SRTF", q) {

}

void S_Scheduler::add_to_queue(Process *p) {
	list <Process *>::iterator it;
	for (it = runQ.begin() ; it != runQ.end(); it++) {
		if ( (*it)->GetRem() > p->GetRem() ) {
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

