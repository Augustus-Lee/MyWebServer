#include "threadpool.h"

//一次性销毁/添加都为两个
const int NUMBER = 2;


//TaskQueue
TaskQueue::TaskQueue()
{
	pthread_mutex_init(&m_mutex, NULL);
}


TaskQueue::~TaskQueue()
{
	pthread_mutex_destroy(&m_mutex);
}


void TaskQueue::addTask(Task& task)
{
	pthread_mutex_lock(&m_mutex);
	m_Queue.push(task);
	pthread_mutex_unlock(&m_mutex);
}


void TaskQueue::addTask(callback func, void* arg)
{
	pthread_mutex_lock(&m_mutex);
	Task task;
	task.function = func;
	task.arg = arg;
	m_Queue.push(task);
	pthread_mutex_unlock(&m_mutex);
}


Task TaskQueue::takeTask()
{
	Task task;
	pthread_mutex_lock(&m_mutex);
	if(m_Queue.size() > 0)
	{
		task = m_Queue.front();
		m_Queue.pop();
	}
	pthread_mutex_unlock(&m_mutex);
	return task;
}


//threadpool
ThreadPool::ThreadPool(int minNum, int maxNum)
{
	//实例化任务队列
	m_taskQueue = new TaskQueue;

	do
	{
		//初始化
		m_minNum = minNum;
		m_maxNum = maxNum;
		m_liveNum = minNum;
		m_busyNum = 0;
		m_exitNum = 0;

		m_workThreadIDS = new pthread_t[maxNum];
		if(m_workThreadIDS == nullptr)
		{
			cout << "m_workThreadIDS alloc fail..\n";
			break;
		}
		memset(m_workThreadIDS, 0, sizeof(pthread_t) * maxNum);

		if(pthread_mutex_init(&m_mutex,NULL) != 0 || pthread_cond_init(&m_isEmpty, NULL) != 0)
		{
			cout << "m_mutex or m_isEmpty init fail..\n";
			break;
		}

		//this指针是所有成员函数的隐含参数 可以用来指向调用对象
		pthread_create(&m_manageThread, NULL, manageFunc, this);

		for(int i = 0; i < minNum; i++)
		{
			pthread_create(&m_workThreadIDS[i], NULL, workFunc, this);
			cout << "create thread ID: " << m_workThreadIDS[i] << endl;
		}
		
	} while (0);

}


void* ThreadPool::manageFunc(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);

	//线程池未关闭时
	while(!pool->m_shutdown)
	{
		sleep(3);
		pthread_mutex_lock(&pool->m_mutex);
		int liveNum = pool->m_liveNum;
		int queueSize = pool->m_taskQueue->getTaskNum();
		int busyNum = pool->m_busyNum;
		pthread_mutex_unlock(&pool->m_mutex);

		//添加线程  
		//任务个数>存活的线程数 && 存活数<最大数
		if(queueSize > liveNum && liveNum < pool->m_maxNum)
		{
			
			pthread_mutex_lock(&pool->m_mutex);
			int count = 0;
			for(int i =0; i < pool->m_maxNum && count < NUMBER && pool->m_liveNum< pool->m_maxNum; i++)
			{
				//当前线程没有存储线程ID时
				if(pool->m_workThreadIDS[i] == 0)
				{
					pthread_create(&pool->m_workThreadIDS[i], NULL, workFunc, pool);
					pool->m_liveNum++;
					count++;
				}

			}
			pthread_mutex_unlock(&pool->m_mutex);
		}

		//销毁线程
		//忙的线程数*2 < 存活线程数 && 存活线程数 > 最小线程数
		if(busyNum * 2 < liveNum && liveNum > pool->m_minNum)
		{
			pthread_mutex_lock(&pool->m_mutex);
			pool->m_exitNum = NUMBER;
			pthread_mutex_unlock(&pool->m_mutex);

			for(int i = 0; i < NUMBER; i++)
			{
				//唤醒阻塞的线程  自己去销毁
				pthread_cond_signal(&pool->m_isEmpty);
			}
		}
	}
	return nullptr;
}


void* ThreadPool::workFunc(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);

	while(true)
	{
		pthread_mutex_lock(&pool->m_mutex);
		//任务队列为空则阻塞
		while(pool->m_taskQueue->getTaskNum() == 0 && !pool->m_shutdown)
		{
			//任务队列为空 阻塞工作线程消费
			pthread_cond_wait(&pool->m_isEmpty, &pool->m_mutex);
			//若需要销毁线程
			if(pool->m_exitNum > 0)
			{
				pool->m_exitNum--;

				if(pool->m_liveNum > pool->m_minNum)
				{
					pool->m_liveNum--;
					pthread_mutex_unlock(&pool->m_mutex);
					pool->threadExit();
				}
			}
		}

		//线程池是否关闭
		if(pool->m_shutdown)
		{
			pthread_mutex_unlock(&pool->m_mutex);
			pool->threadExit();
		}
		//从任务队列中取出一个任务
		Task task = pool->m_taskQueue->takeTask();
		pool->m_busyNum++;
		pthread_mutex_unlock(&pool->m_mutex);

		cout << "thread: " << pthread_self() << " start working..\n";
		//函数指针
		task.function(task.arg);
		delete task.arg;
		task.arg = nullptr;
		cout << "thread: " << pthread_self() << " end working..\n";

		pthread_mutex_lock(&pool->m_mutex);
		pool->m_busyNum--;
		pthread_mutex_unlock(&pool->m_mutex);
	}

	return nullptr;
}


void ThreadPool::threadPoolAdd(Task task)
{
	if(m_shutdown)
	{
		return;
	}
	//任务队列中有锁
	m_taskQueue->addTask(task);
	//唤醒工作线程
	pthread_cond_signal(&m_isEmpty);
}


ThreadPool::~ThreadPool()
{
	m_shutdown = true;
	//回收管理者线程
	pthread_join(m_manageThread, NULL);
	//唤醒工作线程
	for(int i = 0; i < m_liveNum; i++)
	{
		pthread_cond_signal(&m_isEmpty);
	}

	if(m_taskQueue) delete m_taskQueue;
	if(m_workThreadIDS) delete[] m_workThreadIDS;
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_isEmpty);

}


void ThreadPool::threadExit()
{
	pthread_t tid = pthread_self();
	for(int i = 0; i <= m_maxNum; i++)
	{
		if(m_workThreadIDS[i] == tid)
		{
			m_workThreadIDS[i] = 0;
			cout << "threadExit():thread " << tid << "has destroy!\n";
			break;
		}
	}
	pthread_exit(NULL);
}


