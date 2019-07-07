/*
 * Process.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: sungw
 */

#include <iostream>
#include "Process.h"
using namespace std;

Process::Process (int input[]) {
	num = pNum++;
	state_ts = input[0];
	tCPU = input[1];
	CPU_Burst = input[2];
	IO_Burst = input[3];
//	state_ts = 0;
	totalCPU = 0;
	totalIO = 0;
	static_prio = input[4];
	cout << state_ts << " " << tCPU << " " << CPU_Burst << " " << IO_Burst << endl;
}

//int Process::GetArrivalTime() {
//	return arrival;
//}
int Process::GetTimeStamp() {
	return state_ts;
}
void Process::AddTime(int ts) {
	state_ts += ts;
}
int Process::GetNum() {
	return num;
}
int Process::GetCPUBurst() {
	return CPU_Burst;
}
int Process::GetIOBurst() {
	return IO_Burst;
}
int Process::GetRem() {
	return tCPU - totalCPU;
}
int Process::GetPrio() {
	return static_prio;
}
int Process::GetTotalCPU() {
	return totalCPU;
}
void Process::AddCPUTime(int ct) {
	totalCPU += ct;
	AddTime(ct);
}
int Process::GetTotalIO() {
	return totalIO;
}
void Process::AddIOTime(int it) {
	totalIO += it;
	AddTime(it);
}
