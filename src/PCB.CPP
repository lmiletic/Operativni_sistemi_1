#include"PCB.H"
#include<dos.h>
#include"SYSTEM.H"
#include<iostream.h>

volatile PCB* PCB::running=0;
volatile ID PCB::initId=0;
PCB* PCB::mainThread=0;
Idle* PCB::idle=0;



PCB::PCB(Thread *myThread,StackSize stackSize, Time timeSlice) {

	lock
	this->myThread=myThread;
	this->timeSlice=timeSlice;
	this->timeLeft=timeSlice;
	this->id=PCB::initId++;
	this->forceWakeUp=0;
	if(stackSize>65535)stackSize=65535;
	this->size=stackSize/sizeof(unsigned);
	this->stack=new unsigned[size];
	this->blockList=new List();
	createProcess();
    unlock

}
//PROVERI SVE OPET
void PCB::createProcess() {



#ifndef BCC_BLOCK_IGNORE
		stack[size-1] = FP_SEG(myThread);
		stack[size-2] = FP_OFF(myThread);
		stack[size-5] = 0x200;

		stack[size-6] = FP_SEG(PCB::wrapper);
		stack[size-7] = FP_OFF(PCB::wrapper);

		ss = FP_SEG(stack+size-16);
		sp = FP_OFF(stack+size-16);
		bp = FP_OFF(stack+size-16);

#endif

		state=PCB::Created;
		((List*) System::allThreads)->add(this);

}

void PCB::wrapper(){
	PCB::running->myThread->run();
	((List*)running->blockList)->wakeUpAll();
	PCB::running->state=PCB::Terminated;
	dispatch();
}

PCB::~PCB(){
	lock
	((List*)System::allThreads)->remove(this->id);
	delete blockList;
	delete stack;
	unlock
}
