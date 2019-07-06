/*
 * Process.h
 *
 *  Created on: Jul 4, 2019
 *      Author: sungw
 */

#ifndef PROCESS_H_
#define PROCESS_H_

typedef enum {STATE_CREATED, STATE_READY, STATE_RUNNING, STATE_BLOCKED} process_state_t;

class Process {
private:
	int num;
	int arrival;
	int tCPU;
	int CPU_Burst;
	int IO_Burst;
	int state_ts;
	int totalCPU;
	int totalIO;
	int static_prio;
public:
	Process (int input[]);
	static int pNum;
	int TimeStamp();
	int GetNum();
	int GetCPUBurst();
	int GetIOBurst();
};


#endif /* PROCESS_H_ */
