#include"SEMAPHOR.H"
#include"KERNELSE.H"
#include"SYSTEM.H"

Semaphore::Semaphore (int init){
	lock
	myImpl=new KernelSem(this,init);
	unlock
}
Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::wait (Time maxTimeToWait){
	lock
	int ret=myImpl->wait(maxTimeToWait);
	unlock
	return ret;
}
int Semaphore::signal(int n){
	lock
	int ret=myImpl->signal(n);
	unlock
	return ret;
}

int Semaphore::val()const{
	return myImpl->val;
}


