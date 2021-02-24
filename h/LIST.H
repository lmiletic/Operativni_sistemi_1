#ifndef LIST_H_
#define LIST_H_
#include"PCB.H"
#include"THREAD.H"
class List{

private:

	friend class PCB;

	struct Elem {
		PCB* pcb;
		Elem* next;
		Time time;
		Elem(PCB* pcb) :pcb(pcb), next(0),time(0) {}
	};

	Elem *first;
	Elem *last;
protected:
	friend class KernelSem;

public:


	List():first(0),last(0){};

	List* add(PCB *pcb);
	List* addtoSem(PCB *pcb,Time t);
	PCB* remove(ID id);
	PCB* remove();
	PCB* sleepRemove();
	int updateSleep();//koristi updateListOnTick
	void wakeUpAll();
	PCB* take(ID id);
	void print();
	int size();
	virtual ~List();


};





#endif /* LIST_H_ */
