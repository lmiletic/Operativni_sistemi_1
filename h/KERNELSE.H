#ifndef KERNELSE_H_
#define KERNELSE_H_
#include"SEMAPHOR.H"
#include"THREAD.H"
#include"SEMLIST.H"
#include"LIST.H"

class KernelSem{

public:
	static volatile SemList* allSems;

	KernelSem(Semaphore* mySem,int val);
	virtual ~KernelSem();

	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n=0);



protected:
	friend class PCB;
	friend class Semaphore;
	friend class SemList;
	friend class List;
	static ID semID;
	ID id;
	int val;
	List* blocked;
	List* sleeping;
	Semaphore* mySem;


};





#endif /* KERNELSE_H_ */
