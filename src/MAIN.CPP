#include"THREAD.H"
#include"SYSTEM.H"
#include"KERNELSE.H"
#include"LIST.H"
#include"IVTENTRY.H"
#include"SEMLIST.H"
#include<iostream.h>

IVTEntry* IVTEntry::IVT[256];
volatile List* System::allThreads=new List();
volatile SemList* KernelSem::allSems=new SemList();

extern int userMain(int argc,char* argv[]);

int main(int argc,char* argv[]){

	System::inic();


	int user=userMain(argc,argv);

	System::restore();

	//cout<<"Happy END!"<<endl;


	return user;

}
