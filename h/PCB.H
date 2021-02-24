// File: pcb.h

#ifndef PCB_H_
#define PCB_H_


#include"LIST.H"
#include"THREAD.H"
#include"IDLE.H"



class PCB{
public:
	enum State{
		Created=0,
		Ready,
		Blocked,
		Terminated
	};

	static volatile PCB* running;

	PCB(Thread *mythread,StackSize stackSize, Time timeSlice);



	virtual ~PCB();

protected:
	friend class KernelSem;
	friend class List;
	friend class Thread;
	friend class System;
	friend class Idle;
	friend class KernelEv;

	static volatile ID initId;

	ID id;
	volatile List* blockList;
	static PCB* mainThread;
	static Idle* idle;
	volatile State state;
	volatile Time timeLeft;
	volatile Time timeSlice;
	StackSize size;

	unsigned *stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;

	unsigned forceWakeUp;

	Thread *myThread;

	void createProcess();

	static void wrapper();

};


#endif /* PCB_H_ */
