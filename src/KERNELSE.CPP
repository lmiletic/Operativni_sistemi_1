#include"KERNELSE.H"
#include"PCB.H"
#include"SCHEDULE.H"
#include"SYSTEM.H"


ID KernelSem::semID = 0;

KernelSem::KernelSem(Semaphore* mySem, int val) {
	this->id = KernelSem::semID++;
	this->mySem = mySem;
	this->val = val;
	this->blocked = new List();
	this->sleeping = new List();
	((SemList*) KernelSem::allSems)->add(this);
}

KernelSem::~KernelSem() {
	while(val<0){
		dispatch();
	}
	lock
	delete blocked;
	delete sleeping;
	((SemList*)KernelSem::allSems)->remove(this->id);
	unlock
}

int KernelSem::wait(Time maxTimeToWait) {
	val--;
	if (val < 0) {
		PCB::running->state=PCB::Blocked;
		if (maxTimeToWait > 0) {
			//blok na odredjeno vreme
			sleeping->addtoSem((PCB*)PCB::running,maxTimeToWait);
			//sleeping->print();
			dispatch();
			//ispituje da li je prinudno probudjena nit ili ne
			if(((PCB*)PCB::running)->forceWakeUp==1){
				((PCB*)PCB::running)->forceWakeUp=0;
			    return 1;
			}else
				return 0;
		} else {
			//blok na neodredjeno vreme
			blocked->add((PCB*)PCB::running);
			//blocked->print();
			dispatch();
		}
	}
	return 1;
}

int KernelSem::signal(int n) {
	PCB* removed = 0;
	if (n > 0) {
		int unblocked = 0;
		while (n != unblocked) {
			removed = blocked->remove();
			if (removed == 0)
				break;
			unblocked++;
			removed->state=PCB::Ready;
			Scheduler::put(removed);
		}
		while (n != unblocked) {
			removed = sleeping->sleepRemove();
			if (removed == 0)
				break;
			unblocked++;
			removed->forceWakeUp=1;
			removed->state=PCB::Ready;
			Scheduler::put(removed);
		}
		val += n;
		//odblokirati n niti ili koliko maks moze
		return unblocked;
	} else if (n == 0) {
		val++;
		if (val <= 0) {
			removed = blocked->remove();
			if (removed == 0){
				removed = sleeping->sleepRemove();
			    if(removed!=0)
			    	removed->forceWakeUp=1;
			}
			removed->state=PCB::Ready;
			Scheduler::put(removed);

		}
		//standardan signal
		return 0;
	} else
		return n;
}

