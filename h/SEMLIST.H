#ifndef SEMLIST_H_
#define SEMLIST_H_
#include"THREAD.H"

class SemList{

private:

	friend class KernelSem;

	struct Elem {
		KernelSem* sem;
		Elem* next;
		Elem(KernelSem* sem) :sem(sem), next(0) {}
	};

	Elem *first;

public:


	SemList():first(0){};
	void updateListOnTick();
	SemList* add(KernelSem *sem);
	KernelSem* remove();
	KernelSem* remove(ID id);
	void print();
	int size();
	virtual ~SemList();


};





#endif
