#include"THREAD.H"
#include"SYSTEM.H"
#include"LIST.H"
#include"SCHEDULE.H"
#include"IDLE.H"
#include<iostream.h>


    Thread::Thread(StackSize stackSize, Time timeSlice){
    	lock
		myPCB=new PCB(this,stackSize,timeSlice);
    	unlock
    }

    void Thread::start(){
    	if(myPCB->state==PCB::Created){
    		myPCB->state=PCB::Ready;
    		Scheduler::put(myPCB);
    	}
    }
	void Thread::waitToComplete(){
		if(myPCB->state==PCB::Terminated || myPCB==PCB::running || this==PCB::idle)return;
		PCB::running->state=PCB::Blocked;
		((List*)myPCB->blockList)->add((PCB*)PCB::running);
		dispatch();
	}
	 Thread::~Thread(){
		lock
		waitToComplete();
		((List*)System::allThreads)->remove(myPCB->id);
		delete myPCB;
		unlock
	}

	ID Thread::getId(){
		return myPCB->id;
	}
	ID Thread::getRunningId(){
		return PCB::running->id;
	}
	Thread * Thread::getThreadById(ID id){
		PCB* requested=((List*)System::allThreads)->take(id);
		if(requested!=0)return requested->myThread;
		else return 0;
	}

	void dispatch(){
		lock
		System::requested_context_switch=1;
        System::timer();
		unlock
	}


