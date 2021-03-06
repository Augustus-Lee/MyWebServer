#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <cstdlib>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>
#include <queue>

using namespace std;


//任务类
//函数指针 
using callback = void(*)(void*);
template <class T>
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
		this->arg = (T*)arg;
	}

	callback function;
	T* arg;
};


//任务队列类
template <class T>
class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue();

	//添加任务
	void addTask(Task<T> task);
	void addTask(callback func, void* arg);

	//取出任务
	Task<T> takeTask();

	//获取任务当前数量
	inline int getTaskNum()
	{
		return m_Queue.size();
	}

private:
	pthread_mutex_t m_mutex;//互斥锁
	queue<Task<T> > m_Queue;//任务队列

};


//线程池类
template <class T>
class ThreadPool
{
public:
	//创建线程池
	ThreadPool(int minNum, int maxNum);
	
	//销毁线程池
	~ThreadPool();

	//添加任务
	void threadPoolAdd(Task<T> task);

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
	TaskQueue<T>* m_taskQueue;
	
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

	bool m_shutdown = false;//是否关闭线程池 1是 0否
};
#endif //_THREADPOOL_H