#ifndef IDLE_H_
#define IDLE_H_
#include"THREAD.H"


class Idle:public Thread{
public:
	friend class Thread;
	Idle();
	virtual void run();
	void start();
};




#endif /* IDLE_H_ */
