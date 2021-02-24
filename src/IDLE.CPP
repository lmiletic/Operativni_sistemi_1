#include"IDLE.H"
#include"THREAD.H"
#include"SYSTEM.H"
#include<iostream.h>

Idle::Idle():Thread(defaultStackSize,1){


}

void Idle::run(){
	//radi nesto bez veze
	volatile unsigned int a=10;
	/*
	lock
	cout<<"IDLE"<<endl;
	unlock
	*/
	while(a){
		a++;
		a=a+2;
		if(a>100)a=10;
	}
}

void Idle::start(){
	//nikako ne stavljati u SCHEUDLER

}



