#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//任务结构体  添加 删除任务
typedef struct Task
{
	void (*function)(void* arg);
	void *arg;
	
}Task;


//线程池结构体
typedef struct ThreadPool
{
	//任务队列
	Task* taskQ;

	int queueCapacity; 			//容量
	int queueSize;				//当前任务个数
	int queueFront;				//队列头部
	int queueRear;				//队列尾部

	//线程池
	ThreadPool manageThread; 	//管理者线程 只有1个
	ThreadPool* workThreads;	//工作线程  n个
	int maxNum;					//最多线程数
	int minNum;					//最少线程数
	int busyNum;				//忙线程数
	int liveNum;				//存活的线程个数
	int exitNum					//待销毁的线程个数		
	

	//互斥锁  条件变量
	pthread_mutex_t mutexPool;   //锁住整个线程池
	pthread_mutex_t mutexBusy;	 //锁住忙线程
	pthread_cond_t  isFull;		 //任务队列是否已满
	pthread_cond_t  isEmpty;	 //任务队列是否为空	
	
}ThreadPool;


//创建线程池并初始化
ThreadPool* threadPoolCreate(int minNum, int maxNum, int queueCapacity);

void threadPoolAdd();

int getBusyThreadNum(ThreadPool* pool);

int getLiveThreadNum(ThreadPool* pool);






#endif