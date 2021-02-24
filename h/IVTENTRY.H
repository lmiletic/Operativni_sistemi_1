#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include"SYSTEM.H"

typedef unsigned char IVTNo;
class Event;

class IVTEntry{
public:
	IVTEntry(IVTNo n,InterruptRoutine routine);
	virtual ~IVTEntry();

	void callOld();
	void signal();

	static IVTEntry* IVT[256];
protected:
	friend class Event;
	friend class KernelEv;
	Event* myEvent;
private:
	InterruptRoutine oldRoutine;
	IVTNo entryNum;


};




#endif
