/*
 * R_Scheduler.h
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */

#ifndef R_SCHEDULER_H_
#define R_SCHEDULER_H_

#include "Scheduler.h"

class R_Scheduler : public Scheduler {
private:
	 queue <Process *> runQ;
public:
	R_Scheduler(int q);
	~R_Scheduler() {};
	void add_to_queue(Process *p);
	Process* get_next_process();
};



#endif /* R_SCHEDULER_H_ */
