#ifndef KERNELEV_H_
#define KERNELEV_H_
#include"EVENT.H"
#include"IVTENTRY.H"

#define PREPAREENTRY(numEntry,flag)\
	void interrupt inter##numEntry(...);\
	IVTEntry newEntry##numEntry(numEntry,inter##numEntry);\
	void interrupt inter##numEntry(...){\
		newEntry##numEntry.signal();\
		if(flag==1)\
		   newEntry##numEntry.callOld();\
	}

class PCB;

class KernelEv{

public:
	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();

	void wait();
	void signal();
	static void setEvent(Event* e,KernelEv* k);
protected:
	friend class IVTEntry;
	friend class Event;

private:
	unsigned int blocked;
	IVTEntry* myEntry;
	IVTNo myIVTNo;
	PCB* myPCB;
	int val;
};



#endif
