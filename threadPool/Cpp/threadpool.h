#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <pthread.h>

using namespace std;


//任务队列类
class Task
{
public:


	Task();
	~Task();
private:
	int queueSize;
	int queueCapacity;
	int queueRear;
	int queueFront;
};


//线程池类
class ThreadPool
{
public:
	//创建线程池
	ThreadPool();
	
	//销毁线程池
	~ThreadPool();

	//添加任务
	void threadPoolAdd();


	
	
private:
	//管理者线程
	void* manageFunc();

	//工作线程
	void* workFunc();


private:
	//任务队列
	Task* taskQueue;
	

	//线程池
	pthread_t manageThread;
	pthread_t* workThreadIDS;//数组 有若干个工作线程
	int minNum;
	int maxNum;
	int liveNum;
	int busyNum;
	int exitNum;

	//互斥锁及条件变量
	pthread_mutex_t mutexPool;
	pthread_mutex_t mutexBusy;
	pthread_cond_t isFull;
	pthread_cond_t isEmpty;


	bool shutdown = false;//是否关闭线程池 1是 0否
};







#endif //_THREADPOOL_H