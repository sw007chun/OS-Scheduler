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
	int arrival;	//arrival time
	int tCPU;		//total CPU time
	int max_CPU_Burst;
	int leftCPU_Burst;
	int IO_Burst;
	int state_ts;	//timestamp
	int totalCPU;	//total CPU time till now
	int totalIO;	//total IO time till now
	int static_prio;	//static priority
	int dyn_prio;	//dynamic priority
public:
	Process (int input[]);
	static int pNum;
	int GetArrivalTime();
	int GetTimeStamp();
	void AddTime(int ts);
	int GetNum();
	int GetCPUBurst();
	void SetCPUBurst(int cpu);
	void ReduceCPUBurst(int quantum);
	int GetLeftCPUBurst();
	int GetIOBurst();
	int GetRem();
	int GetStatPrio();
	int GetDynPrio();
	int GetTotalCPU();
	void AddCPUTime(int ct);
	int GetTotalIO();
	void AddIOTime(int it);
	void Reduce_Prio();
	void Reset_Prio();
};

#endif /* PROCESS_H_ */
