#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <queue>

using namespace std;


//任务类
//函数指针 
using callback = void(*)(void*);
class Task
{
public:
	Task()
	{
		function = nullptr;
		arg = nullptr;
	}

	Task(callback func, void* arg)
	{
		function = func;
		this->arg = arg;
	}
private:
	callback function;
	void* arg;
};


//任务队列类
class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue();

	//添加任务
	void addTask(Task& task);
	void addTask(callback func, void* arg);

	//取出任务
	Task takeTask();

	//获取任务当前数量
	inline int getTaskNum()
	{
		return m_Queue.size();
	}

private:
	pthread_mutex_t m_mutex;//互斥锁
	queue<Task> m_Queue;//任务队列

};


//线程池类
class ThreadPool
{
public:
	//创建线程池
	ThreadPool(int minNum, int maxNum);
	
	//销毁线程池
	~ThreadPool();

	//添加任务
	void threadPoolAdd(Task task);

private:
	//静态成员函数没有this指针  故不会作为函数参数传入
	//管理者线程
	static void* manageFunc(void* arg);

	//工作线程
	static void* workFunc(void* arg);

	//线程退出
	void threadExit();

private:
	//任务队列
	TaskQueue* m_taskQueue;
	
	//线程池
	pthread_t m_manageThread;
	pthread_t* m_workThreadIDS;//数组 有若干个工作线程
	int m_minNum;
	int m_maxNum;
	int m_liveNum;
	int m_busyNum;
	int m_exitNum;

	//互斥锁及条件变量
	pthread_mutex_t m_mutex;
	pthread_cond_t m_isEmpty;

	bool m_shutdown = FALSE;//是否关闭线程池 1是 0否
};
#endif //_THREADPOOL_H