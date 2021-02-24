/*
 * SYSTEM.CPP
 *
 *  Created on: Dec 23, 2019
 *      Author: OS1
 */

#include<dos.h>
#include<iostream.h>
#include"SYSTEM.H"
#include"LIST.H"
#include"SCHEDULE.H"
#include"SEMLIST.H"
#include"KERNELSE.H"
#include"SYNCPRI.H"


volatile unsigned tsp,tss,tbp,oldTimerOFF,oldTimerSEG;

InterruptRoutine irTimer=&System::timer;
//volatile List* System::allThreads=0;
volatile unsigned int System::requested_context_switch=0;
volatile unsigned int System::lockFlag=1;
extern void tick();


void interrupt System::timer(...){
/*
	lock
	cout<<"TIMER"<<endl;
	unlock
*/

	if(requested_context_switch==0){
		    tick();
			if(PCB::running->timeSlice!=0)(PCB::running->timeLeft)--;
			((SemList*)KernelSem::allSems)->updateListOnTick();
			//((SemList*)KernelSem::allSems)->print();
			//syncPrintf("TICK\n");
			asm int 60h;
		}

	if((PCB::running->timeLeft==0 && PCB::running->timeSlice!=0) || requested_context_switch){
		if(lockFlag==1){
			requested_context_switch=0;

			asm{
				mov tsp,sp
				mov tss,ss
				mov tbp,bp
			}

			PCB::running->sp=tsp;
			PCB::running->ss=tss;
			PCB::running->bp=tbp;

			if(PCB::running->state==PCB::Ready && ((PCB*)PCB::running)!=(PCB::idle->myPCB))
				Scheduler::put((PCB*)PCB::running);

			PCB::running=Scheduler::get();

			if(PCB::running==0){
				PCB::running=PCB::idle->myPCB;
			}

			tsp=PCB::running->sp;
			tss=PCB::running->ss;
			tbp=PCB::running->bp;

			PCB::running->timeLeft=PCB::running->timeSlice;

			asm{
				mov sp,tsp
				mov ss,tss
				mov bp,tbp
			}

		}else requested_context_switch=1;

	}

}

void System::inic(){

#ifndef BCC_BLOCK_IGNORE
	unsigned timerSeg=FP_SEG(System::timer);
    unsigned timerOff=FP_OFF(System::timer);
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax,word ptr es:0022h
		mov word ptr oldTimerSEG,ax
		mov ax,word ptr es:0020h
		mov word ptr oldTimerOFF,ax


		mov ax,word ptr timerSeg
		mov word ptr es:0022h, ax
		mov ax,word ptr timerOff
		mov word ptr es:0020h, ax

		mov ax,oldTimerSEG
		mov word ptr es:0182h,ax
		mov ax,oldTimerOFF
		mov word ptr es:0180h,ax

		pop ax
		pop es
		sti
	}
#endif

	//Premestam u main
	//System::allThreads=new List();
	//KernelSem::allSems=new SemList();
	PCB::mainThread=new PCB(0,defaultStackSize, defaultTimeSlice);
	PCB::mainThread->state=PCB::Ready;
	PCB::running=PCB::mainThread;
	PCB::idle=new Idle();
}

void System::restore(){

	delete PCB::mainThread;
	delete PCB::idle;
	delete KernelSem::allSems;
	delete System::allThreads;

#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax,word ptr oldTimerSEG
		mov word ptr es:0022h,ax
		mov ax,word ptr oldTimerOFF
		mov word ptr es:0020h,ax

		pop ax
		pop es
		sti
	}
#endif

}









