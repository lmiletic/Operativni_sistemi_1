#include"SEMLIST.H"
#include"KERNELSE.H"
#include"SYSTEM.H"
#include"SYNCPRI.H"
#include<iostream.h>

void SemList::updateListOnTick(){
	Elem* tmp=first;
	int ret=0;//uvecava vrednost semafora za odblokirane
	while(tmp!=0){
		//syncPrintf("SemID%d : ",tmp->sem->id);
		ret=tmp->sem->sleeping->updateSleep();
		if(ret!=0){
			tmp->sem->val+=ret;
		}
		tmp=tmp->next;
	}
	//syncPrintf("Ret %d\n",ret);
}

SemList* SemList::add(KernelSem *sem) {
	lock
	Elem *temp=new Elem(sem);
	if(!first){
		first=temp;
	}
	else{
		temp->next=first;
		first=temp;
	}
	unlock
	return this;
}

KernelSem* SemList::remove(){
	lock
	KernelSem* removed=0;
	Elem* old=first;
	if(old!=0){
	removed=old->sem;
	first=first->next;
	delete old;
	}
	unlock
	return removed;
}

KernelSem* SemList::remove(ID id) {
	lock
	Elem *temp=first,*prev=0;
	KernelSem *removed=0;
	while(temp){
		if(temp->sem->id!=id){
			prev=temp;
			temp=temp->next;
		}
		else{
			Elem *old=temp;
			temp=temp->next;
			if(!prev)first=temp;
			else prev->next=temp;
			removed=old->sem;
			delete old;
			break;
		}
	}
	unlock
	return removed;
}

void SemList::print() {
	lock
	for(Elem *temp=first;temp!=0;temp=temp->next){
		syncPrintf("SemID %d,",temp->sem->id);
	}
	syncPrintf("\n");
	unlock
}

int SemList::size(){
	lock
	Elem *temp=first;
	int num=0;
	while(temp){
		num++;
		temp=temp->next;
	}
	unlock
	return num;
}

SemList::~SemList() {
	lock
	while(first){
		Elem *old=first;
		first=first->next;
		delete old;
	}
	unlock
}

