/*
 * Process.h
 *
 *  Created on: Jul 4, 2019
 *      Author: sungw
 */

#ifndef PROCESS_H_
#define PROCESS_H_

class Process {
private:
	int num;	//Process #
//	int arrival;	//arrival time
	int tCPU;		//total CPU time
	int CPU_Burst;
	int IO_Burst;
	int state_ts;	//timestamp
	int totalCPU;	//total CPU time till now
	int totalIO;	//total IO time till now
	int static_prio;	//static priority
public:
	Process (int input[]);
	static int pNum;
//	int GetArrivalTime();
	int GetTimeStamp();
	void AddTime(int ts);
	int GetNum();
	int GetCPUBurst();
	int GetIOBurst();
	int GetRem();
	int GetPrio();
	int GetTotalCPU();
	void AddCPUTime(int ct);
	int GetTotalIO();
	void AddIOTime(int it);
};


#endif /* PROCESS_H_ */
