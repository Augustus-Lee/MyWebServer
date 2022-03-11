#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


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
	pthread_t manageThread; 	//管理者线程 只有1个
	pthread_t* workThreadIDs;	//工作线程  n个
	int maxNum;					//最多线程数
	int minNum;					//最少线程数
	int busyNum;				//忙线程数
	int liveNum;				//存活的线程个数
	int exitNum;		//待销毁的线程个数		
	
	//互斥锁  条件变量
	pthread_mutex_t mutexPool;   //锁住整个线程池
	pthread_mutex_t mutexBusy;	 //锁住忙线程
	pthread_cond_t  isFull;		 //任务队列是否已满
	pthread_cond_t  isEmpty;	 //任务队列是否为空	

	int shutdown;                //是否销毁线程池，1是，0否
	
}ThreadPool;


//创建线程池并初始化
ThreadPool* threadPoolCreate(int minNum, int maxNum, int queueCapacity);

void threadPoolAdd(ThreadPool* pool, void (*func)(void*), void* arg);

int threadPoolDestory(ThreadPool* pool);

int getBusyThreadNum(ThreadPool* pool);

int getLiveThreadNum(ThreadPool* pool);



//线程
void* manageFunc(void* arg);

void* workFunc(void* arg);

//退出单个线程
void threadExit(ThreadPool* pool);

#endif//_THREADPOOL_H