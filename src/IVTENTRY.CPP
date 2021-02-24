#include"IVTENTRY.H"
#include"SYSTEM.H"
#include"EVENT.H"
#include<dos.h>

IVTEntry::IVTEntry(IVTNo n,InterruptRoutine routine){
	lock
	this->entryNum=n;
	IVTEntry::IVT[entryNum]=this;
	this->myEvent=0;
	oldRoutine=0;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine=getvect(entryNum);
	setvect(entryNum,routine);
#endif
	unlock
}

IVTEntry::~IVTEntry(){
	lock
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNum,oldRoutine);
#endif
	IVTEntry::IVT[entryNum]=0;
	myEvent=0;
	unlock
}
void IVTEntry::callOld(){
	oldRoutine();
}

void IVTEntry::signal(){
	if(myEvent!=0){
		myEvent->signal();
	}
}



