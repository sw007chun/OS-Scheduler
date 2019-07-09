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
	arrival = input[0];
	state_ts = input[0];
	tCPU = input[1];
	max_CPU_Burst = input[2];
	leftCPU_Burst = 0;
	IO_Burst = input[3];
	totalCPU = 0;
	totalIO = 0;
	static_prio = input[4];
	dyn_prio = static_prio - 1;
}

int Process::GetArrivalTime() {
	return arrival;
}
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
	return max_CPU_Burst;
}
void Process::SetCPUBurst(int cpu) {
	leftCPU_Burst = cpu;
}
void Process::ReduceCPUBurst(int quantum) {
	leftCPU_Burst -= quantum;
}
int Process::GetLeftCPUBurst() {
	return leftCPU_Burst;
}
int Process::GetIOBurst() {
	return IO_Burst;
}
int Process::GetRem() {
	return tCPU - totalCPU;
}
int Process::GetStatPrio() {
	return static_prio;
}
int Process::GetDynPrio() {
	return dyn_prio;
}
int Process::GetTotalCPU() {
	return tCPU;
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
void Process::Reduce_Prio() {
	dyn_prio--;
}
void Process::Reset_Prio() {
	dyn_prio = static_prio - 1;
}
