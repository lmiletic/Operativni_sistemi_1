#include<iostream.h>
#include"LIST.H"
#include"PCB.H"
#include"SYSTEM.H"
#include"SCHEDULE.H"
#include"SYNCPRI.H"

//testiraj addtoSem
List* List::addtoSem(PCB *pcb,Time t){
	lock
	Elem *temp=new Elem(pcb);
	Time cnt=0;
	if(!first){
		first=temp;
		first->time=t;
	}else{
		Elem *curr=first;
		Elem *prev=0;
		while(curr!=0){
			if((cnt+(curr->time))<t){
				cnt+=curr->time;
				prev=curr;
				curr=curr->next;
			}else{
				break;
			}
		}
		if(prev==0){
			first=temp;
		}else{
			prev->next=temp;
		}
		temp->next=curr;
		temp->time=t-cnt;
		if(curr!=0)curr->time=(curr->time)-(temp->time);
	}
	unlock
}

PCB* List::sleepRemove(){
	lock
	//signal vec uvecao val i uklanja se pre isteka vremena
	PCB* removed=0;
	Elem* old=first;
	if(old!=0){
		removed=old->pcb;
		first=first->next;
		if(first!=0)
		   first->time+=old->time;
		delete old;
		}
	unlock
	return removed;
}

int List::updateSleep(){
	//vraca broj obrisanih zbog isteka vremena cekanja,ubaciti u Scheduler
	Elem* tmp=first;
	PCB* removed=0;
	int cnt=0;//broji koliko niti treba odblokirati
	if(tmp!=0){
	   tmp->time--;
	}
	while(tmp!=0 && tmp->time==0){
		cnt++;
		tmp=tmp->next;
	}
	for(int i=0;i<cnt;i++){
		removed=this->sleepRemove();
		if(removed!=0){
			removed->state=PCB::Ready;
			Scheduler::put(removed);
		}
	}
	//this->print();
	return cnt;
}


List* List::add(PCB *pcb) {
	lock
	Elem *temp=new Elem(pcb);
	if(!first){
		first=temp;
		last=temp;
	}
	else{
		last->next=temp;
		last=temp;
	}
	unlock
	return this;
}

PCB* List::remove(){
	lock
	PCB* removed=0;
	Elem* old=first;
	if(old!=0){
	removed=old->pcb;
	first=first->next;
	if(first==0)last=0;
	delete old;
	}
	unlock
	return removed;
}

void List::wakeUpAll(){
	lock
	PCB* unblocked=0;
	while(1){
		unblocked=this->remove();
		if(unblocked==0)break;
		unblocked->state=PCB::Ready;
		Scheduler::put(unblocked);
	}
	unlock
}


PCB* List::remove(ID id) {
	lock
	Elem *temp=first,*prev=0;
	PCB *removed=0;
	while(temp){
		if(temp->pcb->id!=id){
			prev=temp;
			temp=temp->next;
		}
		else{
			Elem *old=temp;
			temp=temp->next;
			if(!prev)first=temp;
			else prev->next=temp;
			if(last==old)last=prev;
			removed=old->pcb;
			delete old;
			break;
		}
	}
	unlock
	return removed;
}

PCB* List::take(ID id) {
	    lock
		Elem *temp=0;
		PCB *taken=0;
		for(temp=first;temp->pcb->id!=id;temp=temp->next);
		if(temp){
			taken=temp->pcb;
		}
		unlock
		return taken;
}

void List::print() {
	lock
	for(Elem *temp=first;temp!=0;temp=temp->next){
		syncPrintf("Id %d T %d, ",temp->pcb->id,temp->time);
	}
	syncPrintf("\n");
	unlock
}

int List::size(){
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

List::~List() {
	lock
	while(first){
		Elem *old=first;
		first=first->next;
		delete old;
	}
	last=0;
	unlock
}

