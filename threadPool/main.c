//----------------------------------------------------------------------
//--------    Author: AA                                                
//--------    DateTime: 2022-03-08
//--------    Description: ThreadPool                             
//----------------------------------------------------------------------
#include "threadpool.h"

void taskFunc(void *arg)
{
    //强转之后 解引用
    int num = *(int*)arg;
    printf("thread %ld is working, num:%d.\n", pthread_self(), num);
    sleep(1);
}

int main()
{
    //创建线程池
    ThreadPool* pool = threadPoolCreate(3, 10, 100);
    int testNum = 1;
    for(int i = 0; i != 100; i++)
    {
        printf("testNum:%d.\n", testNum);
        testNum++;
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        threadPoolAdd(pool, taskFunc, num);
    }
    //执行10s
    sleep(10);
    threadPoolDestory(pool);

    return 0;
}