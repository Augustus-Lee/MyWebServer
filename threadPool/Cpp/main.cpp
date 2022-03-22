//----------------------------------------
//--------    Author: AA
//--------    DateTime: 2022-03-11
//--------    Description: Cpp threadpool
//----------------------------------------
#include "threadpool.h"
#include "threadpool.cpp"

void taskFunc(void* arg)
{
	int num = *(static_cast<int*>(arg));
	cout << "pthread " << pthread_self() << " is working, num:" << num << endl;
	sleep(1);
}


int main()
{
	ThreadPool<int> pool(5, 20);
	for (int i = 0; i < 100; i++)
	{
		int* num = new int(i + 100);
		pool.threadPoolAdd(Task<int>(taskFunc, num));
	}
	
	sleep(10);
	return 0;
}