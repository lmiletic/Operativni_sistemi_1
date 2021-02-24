#include"EVENT.H"
#include"SYSTEM.H"
#include"KERNELEV.H"

Event::Event(IVTNo ivtNo){
	lock
	this->myImpl=new KernelEv(ivtNo);
	KernelEv::setEvent(this,myImpl);
	unlock
}

Event::~Event(){
	lock
	delete myImpl;
	unlock
}

void Event::wait(){
	lock
	myImpl->wait();
	unlock
}

void Event::signal(){
	lock
	myImpl->signal();
	unlock
}



