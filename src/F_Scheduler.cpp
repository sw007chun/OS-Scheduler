/*
 * F_Scheduler.cpp
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#include "F_Scheduler.h"

F_Scheduler::F_Scheduler(queue<Process *> *pQ) : Scheduler(pQ) {

}

void F_Scheduler::add_to_queue(Process *p) {
	push(p);
}

//Process* F_Scheduler::get_next_process() {
//	Process * p = allP->front();
//	allP->pop();
//	return p;
//}
