/*
 * E_Scheduler.h
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */

#ifndef E_SCHEDULER_H_
#define E_SCHEDULER_H_

#include "Scheduler.h"

class E_Scheduler : public Scheduler {
private:
	 list <Process *> *activeQ;
	 list <Process *> *expiredQ;
public:
	E_Scheduler(int q);
	~E_Scheduler();
	void add_to_queue(Process *p);
	Process* get_next_process();
};




#endif /* E_SCHEDULER_H_ */
