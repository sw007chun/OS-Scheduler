/*
 * L_Scheduler.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */


#include "L_Scheduler.h"

L_Scheduler::L_Scheduler() : Scheduler("LCFS") {

}

void L_Scheduler::add_to_queue(Process *p) {
	runQ.push(p);
}
Process* L_Scheduler::get_next_process() {
	if (!runQ.empty()) {
		Process * p = runQ.top();
		runQ.pop();
		return p;
	} else
		return NULL;
}
