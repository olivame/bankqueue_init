/* 以下为apqueue.h内容，定义优先级队列 */
#ifndef PRIORITYQUEUE
#define PRIORITYQUEUE

#include <stdio.h>
#include <stdlib.h>

/* 队列最大长度 */
#define MaxPQSize 50

struct pQueue
{
	/* 优先级队列数组和计数器 */
	int count;
	DataType pqlist[MaxPQSize]; //Event
};
typedef struct pQueue PQueue;

void InitPQueue(PQueue* pq);
/* 优先级队列修改操作 */
void PQInsert(PQueue* pq, DataType item);
DataType PQDelete(PQueue* pq);
void ClearPQ(PQueue* pq);
/* 优先级队列检测函数 */
int PQEmpty(PQueue* pq);
int PQFull(PQueue* pq);
int PQLength(PQueue* pq);

/* 初始化优先级队列计数 */
void InitPQueue(PQueue* pq)
{
	pq->count = 0;
}

/* 将元素插入优先级队列 */
void PQInsert(PQueue* pq, DataType item)
{
	/* 如果优先级队列的计数器达到最大限制，则终止程序 */
	if (pq->count == MaxPQSize)
	{
		printf("Priority queue overflow!\n");
		exit(1);
	}
	/* 将元素放在队列的后面并递增计数 */
	pq->pqlist[pq->count] = item;
	pq->count++;
}

/* 从优先级队列中删除一个元素并返回其值 */
DataType PQDelete(PQueue* pq)
{
	DataType min;
	int i, minindex = 0;

	if (pq->count > 0)
	{
		/* 在优先级队列中查找最小值及其索引 */
		min = pq->pqlist[0];   /* 假设pqlist[0]是最小值 */

		for (i = 1; i < pq->count; i++)
			if (CompareEvent(&pq->pqlist[i], &min))
			{
				min = pq->pqlist[i];
				minindex = i;
			}
		/* 将第二小元素移动到minindex并递减计数 */
		pq->pqlist[minindex] = pq->pqlist[pq->count - 1];
		pq->count--;
	}
	/* qlist为空，终止程序 */
	else
	{
		printf("Deleting from an empty priority queue!\n");
		exit(1);
	}
	/* 返回最小值 */
	return min;
}

/* 返回优先级队列元素个数 */
int PQLength(PQueue* pq)
{
	return pq->count;
}

/* 测试优先级队列是否为空 */
int PQEmpty(PQueue* pq)
{
	return pq->count == 0;
}

/* 测试优先级队列是否已满 */
int PQFull(PQueue* pq)
{
	return pq->count == MaxPQSize;
}

/* 通过将计数重置为0来清除优先级队列 */
void ClearPQ(PQueue* pq)
{
	pq->count = 0;
}

#endif
