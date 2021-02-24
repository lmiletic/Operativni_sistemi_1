/*
 * SYSTEM.H
 *
 *  Created on: Dec 17, 2019
 *      Author: OS1
 */
#ifndef SYSTEM_H_
#define SYSTEM_H_
#include<dos.h>
#include"LIST.H"
#include"THREAD.H"


#define lock asm pushf;asm sti;

#define unlock asm popf;

typedef void interrupt(*InterruptRoutine)(...);
extern void tick();


class System{
public:

	friend class PCB;
	friend class Thread;
	friend class Idle;


	static volatile List* allThreads;
	static volatile unsigned int requested_context_switch;
	static volatile unsigned int lockFlag;


	static void interrupt timer(...);
	static InterruptRoutine irTimer;

	static void inic();
	static void restore();

private:

	//static volatile unsigned tsp,tss,tbp;
	//static volatile unsigned oldTimerOFF, oldTimerSEG;


};

#endif /* SYSTEM_H_ */
