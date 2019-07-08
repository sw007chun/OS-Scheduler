/*
 * F_Scheduler.h
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef F_SCHEDULER_H_
#define F_SCHEDULER_H_

#include "Scheduler.h"

class F_Scheduler : public Scheduler {
public:
	F_Scheduler();
	void add_to_queue(Process *p);
	Process* get_next_process();
};


#endif /* F_SCHEDULER_H_ */
