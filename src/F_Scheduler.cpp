/*
 * F_Scheduler.cpp
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#include "F_Scheduler.h"

F_Scheduler::F_Scheduler() : Scheduler("FCFS") {

}
void F_Scheduler::add_to_queue(Process *p) {
	runQ.push(p);
}
Process* F_Scheduler::get_next_process() {
	if (!runQ.empty()) {
		Process * p = runQ.front();
		runQ.pop();
		return p;
	} else
		return NULL;
}
