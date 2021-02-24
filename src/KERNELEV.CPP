#include"KERNELEV.H"
#include"PCB.H"
#include"IVTENTRY.H"
#include"SCHEDULE.H"

KernelEv::KernelEv(IVTNo ivtNo){
	this->myIVTNo=ivtNo;
	this->myPCB=(PCB*)PCB::running;
	this->myEntry=IVTEntry::IVT[myIVTNo];
	this->val=0;
	this->blocked=0;

}

KernelEv::~KernelEv(){
	this->myPCB=0;
	IVTEntry::IVT[myIVTNo]=0;
}

void KernelEv::wait(){
	if(myPCB==(PCB*)PCB::running){
		if(val==1){
			val=0;
		}else{
			blocked=1;
			PCB::running->state=PCB::Blocked;
			dispatch();
		}
	}
}

void KernelEv::signal(){
	if(val==1)return;
	if(blocked==1){
		blocked=0;
		myPCB->state=PCB::Ready;
		Scheduler::put(myPCB);
	}else{
		val=1;
	}
}

void KernelEv::setEvent(Event* e,KernelEv* k){
	k->myEntry->myEvent=e;
}
