#include "threadpool.h"
#include <string.h>

//一次向添加或者销毁线程数
const int NUMBER = 2;


ThreadPool* threadPoolCreate(int minNum, int maxNum, int queueCapacity)
{
	ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));

	do
	{
		if(pool == NULL)
		{
			printf("%malloc ThreadPool fail!\n");
			break;
		}
	
		pool->workThreadIDs = (pthread_t*)malloc(sizeof(pthread_t) * max);
		if(pool->workThreadIDs == NULL)
		{
			printf("%malloc workThreadIDs fail!\n");
			break;
		}
		//最大容量为max的空间全部设置为0
		memset(pool->workThreadIDs, 0, sizeof(pthread_t) * max);
		pool->maxNum = maxNum;
		pool->minNum = minNum;
		pool->busyNum = 0;
		pool->liveNum = minNum;//存活是最小线程数
		pool->exitNum = 0;

		if(pthread_mutex_init(&mutexPool, NULL) != 0 || pthread_mutex_init(&mutexBusy, NULL != 0)
			pthread_cond_init(&isFull, NULL) != 0 || pthread_cond_init(&isEmpty, NULL) != 0)
		{
			printf("%mutex or cond init fail!\n");
			break;
		}

		pool->taskQ = (Task*)malloc(sizeof(Task) * queueCapacity);
		if(pool->taskQ == NULL)
		{
			printf("%malloc taskQ fail!\n");
			break;
		}
		pool->queueCapacity = queueCapacity;
		//初始化时都为空
		pool->queueSize = 0;
		pool->queueFront = 0;
		pool->queueRear = 0;
		
		//默认否
		pool->shutdown = 0;

		//创建线程
		pthread_create(&pool->manageThread, NULL, manageFunc, pool);

		for(int i = 0; i <= minNum; i++)
		{
			pthread_create(&pool->workThreadIDs[i], NULL, workFunc, pool);
		}

		return pool;

	} while (0);
	
	//释放资源
	if(pool && pool->manageThread) free(pool->manageThread);

	if(pool && pool->workThreadIDs) free(pool->workThreadIDs);

	if(pool) free(pool);


	return NULL;

}

void* workFunc(void* arg)
{
	//参数void* arg是仿照pthread_create穿的参数  可以进行任何类型转换
	ThreadPool* pool = (ThreadPool*)arg;
	
	//一直不停工作
	while(true)
	{
		pthread_mutex_lock(&pool->mutexPool)
		//任务队列为空则阻塞
		while(pool->queueSize == 0 && !pool->shutdown)
		{
			pthread_cond_wait(&pool->isEmpty, &pool->mutexPool);
			//若需要销毁线程
			if(pool->exitNum > 0)
			{
				pool->exitNum--;

				if(pool->liveNum > pool->minNum)
				{
					pool->liveNum--;
					pthread_mutex_unlock(&pool->mutexPool);
					threadExit(pool);
				}
			}
		}

		//线程池是否关闭
		if(shutdown)
		{
			pthread_mutex_unlock(&pool->mutexPool);
			threadExit(pool);
		}

		//从任务队列中取出一个对象  队头取  队尾加
		Task task;
		task.function = pool->taskQ[pool->queueFront].function;
		task.arg = pool->taskQ[pool->queueFront].arg;
		pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
		pool->queueSize--;

		//解锁
		pthread_cond_signal(&pool->isFull);
		pthread_mutex_unlock(&pool->mutexPool);

		//模拟工作线程工作
		printf("thread %ld Start working...\n", pthread_self());
		pthread_mutex_lock(&pool->mutexBusy);
		pool->busyNum++;
		pthread_mutex_unlock(&pool->mutexBusy);
		task.function(task.arg);
		free(task.arg);
		task.arg = NULL;

		printf("thread %ld End working...\n", pthread_self());
		//结束工作  busynum--;
		pthread_mutex_lock(&pool->mutexBusy);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->mutexBusy);

	}

	return NULL;
}


void* manageFunc(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	//未销毁线程池时
	while(!pool->shutdown)
	{
		//每隔3s检查一下
		sleep(3);

		//取出当前线程中的存活数和任务个数
		pthread_mutex_lock(&pool->mutexPool);
		int liveNum = pool->liveNum;
		int queueSize = pool->queueSize;
		pthread_mutex_unlock(&pool->mutexPool);

		pthread_mutex_lock(&pool->mutexBusy);
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->mutexBusy);

		//添加线程  
		//任务个数>存活的线程数 && 存活数<最大数
		if(pool->queueSize > liveNum && liveNum < pool->maxNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			int count = 0;
			for(int i =0; i < pool->maxNum && count < NUMBER && pool->liveNum< pool->maxNum; i++)
			{
				//当前线程没有存储线程ID时
				if(pool->workThreadIDs[i] == 0)
				{
					pthread_create(&workThreadIDs[i], NULL, workFunc, pool);
					pool->liveNum++;
					count++;
				}

			}
			pthread_mutex_unlock(&pool->mutexPool);
		}

		//销毁线程
		//忙的线程数*2 < 存活线程数 && 存活线程数 > 最小线程数
		if(busyNum * 2 < liveNum && liveNum > pool->minNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			mutexPool->exitNum = NUMBER;
			pthread_mutex_unlock(&pool->mutexPool);
			for(int i = 0; i < NUMBER; i++)
			{
				//唤醒阻塞的线程  自己去销毁
				pthread_cond_signal(&pool->isEmpty);
			}

		}

	}

	return NULL;

}


void threadPoolAdd(ThreadPool* pool, void (*func)(void*), void* arg)
{
	//临界区 锁住整个线程池
	pthread_mutex_lock(&mutexPool);
	//任务队列已满时且线程池每销毁时  
	if(pool->queueSize == pool->queueCapacity || !pool->shutdown)
	{
		//阻塞生产者线程
		pthread_cond_wait(&pool->isFull, &pool->mutexPool);
	}

	if(pool->shutdown)
	{
		pthread_mutex_unlock(&pool->mutexPool);
		return;
	}

	//循环数组   添加  rear + 1 % capacity
	pool->taskQ[pool->queueRear].function = func;
	pool->taskQ[pool->queueRear].arg = arg;
	pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
	pool->queueSize++;

	pthread_cond_signal(&pool->isEmpty);
	pthread_mutex_unlock(&mutexPool);

}


int threadPoolDestory(ThreadPool* pool)
{
	if(pool == NULL)
	{
		return -1;
	}

	//关闭线程池
	pool->shutdown = 1;
	//回收管理者线程
	pthread_join(pool->manageThread, NULL);



}


void threadExit(ThreadPool* pool)
{
	pthread_t tid = pthread_self();

	for(int i = 0; i < pool->maxNum; i++)
	{
		if(pool->workThreadIDs[i] == tid)
		{
			pool->workThreadIDs[i];
			printf("tid %ld has destroyed...\n", tid);
			break;
		}
	}

	pthread_exit(NULL);
}


int getBusyThreadNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->mutexBusy);
	int busyNum = pool->busyNum;
	pthread_mutex_unlock(&pool->mutexBusy);
	return busyNum;
}


int getLiveThreadNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->mutexPool);
	int liveNum = pool->liveNum;
	pthread_mutex_unlock(&pool->mutexPool);
	return liveNum;
}










