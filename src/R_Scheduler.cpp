/*
 * R_Scheduler.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */


#include "R_Scheduler.h"

R_Scheduler::R_Scheduler(int q) : Scheduler("RR", q) {

}

void R_Scheduler::add_to_queue(Process *p) {
	runQ.push(p);
}
Process* R_Scheduler::get_next_process() {
	if (!runQ.empty()) {
		Process * p = runQ.front();
		runQ.pop();
		return p;
	} else
		return NULL;
}
