/* 以下均为 sim.h 内容 */
#ifndef SIMULATION
#define SIMULATION

#include <stdio.h>
#include <stdlib.h>
#include "event.h"
typedef Event DataType;  
#include "apqueue.h"

/* 出纳窗口信息 */
struct tellerStats
{
    int finishService;      				/* 空闲时间预告 */
    int totalCustomerCount; 				/* 服务过的客户总数 */
    int totalCustomerWait;  				/* 客户总的等待时间 */
    int totalService;       				/* 总的服务时间 */
};
typedef struct tellerStats TellerStats;

/* 模拟结构数据 */
struct simulation
{
    int simulationLength;        			/* 模拟时间长度 */
    int numTellers;              			/* 出纳窗口数 */
    int nextCustomer;            			/* 下一个客户编码 */
    int arrivalLow, arrivalHigh; 			/* 下一个客户到达的时间区间 */
    int serviceLow, serviceHigh; 			/* 客户需要服务的时间区间 */
    TellerStats tstat[11];       			/* 出纳窗口信息 */
    PQueue pq;                   			/* 优先级队列 */
};
typedef struct simulation Simulation;

void InitSimulation(Simulation* s);			/* 事件驱动模拟的初始化 */
int NextArrivalTime(Simulation* s);     	/* 计算下一客户到达时间，随机生成 */
int Get_ServiceTime(Simulation*);       	/* 计算客户服务时间，随机生成 */
int NextAvailableTeller(Simulation* s,isVip); 	/* 计算下一个可供服务的出纳窗口号 */
void RunSimulation(Simulation* s);			/* 运行事件模拟 */
void PrintSimulationResults(Simulation* s);	/* 输出模拟结果 */

/* 事件驱动模拟的初始化 */
void InitSimulation(Simulation* s)
{
    int i;
    Event* firstevent = (Event*)malloc(sizeof(Event));

    /* 初始化出纳窗口信息，注意这里限制最多10个窗口，并且编号是从1开始的 */
    for (i = 1; i <= 10; i++)
    {
        s->tstat[i].finishService = 0;
        s->tstat[i].totalService = 0;
        s->tstat[i].totalCustomerWait = 0;
        s->tstat[i].totalCustomerCount = 0;
    }
    s->nextCustomer = 1;

    /* 输入事件模拟相关数据 */
    printf("Enter the simulation time in minutes: ");
    scanf_s("%d", &s->simulationLength);
    printf("Enter the number of bank tellers: ");
    scanf_s("%d", &s->numTellers);  		/* 注意这里不能超过10个 */
    printf("Enter the range of arrival times in minutes: ");
    scanf_s("%d%d", &s->arrivalLow, &s->arrivalHigh);
    printf("Enter the range of service times in minutes: ");
    scanf_s("%d%d", &s->serviceLow, &s->serviceHigh); 

    /* 生成第一个到达事件并加入事件优先级队列 */
    InitEvent(firstevent, 0, arrival, 1, 0, 0, 0, (rand() % 4 == 0) ? Vip : notVip);
    InitPQueue(&(s->pq));
    PQInsert(&(s->pq), *firstevent);
}

/* 计算下一客户到达时间，按照所给区间随机生成 */
int NextArrivalTime(Simulation* s)
{
    return s->arrivalLow + rand() % (s->arrivalHigh - s->arrivalLow + 1);
}

/* 计算客户的服务时间，按照所给区间随机生成 */
int Get_ServiceTime(Simulation* s)
{
    return s->serviceLow + rand() % (s->serviceHigh - s->serviceLow + 1);
}

/* 计算下一个可供服务的出纳窗口号 */
int NextAvailableTeller(Simulation* s,isVip status)
{ 
    int minfinishindex = 1;						/* 初始化最快空闲窗口为第一个窗口 */
    int minfinish = s->tstat[1].finishService;	/* 初始化最快空闲时间预告为第一个窗口的空闲时间预告 */
    int num[1000], m, i;
	
    /* 遍历所有窗口，找到最小空闲时间预告以及对应的窗口编号，如果存在多个最小窗口则随机选择其中一个 */
    m = 1;
    num[0] = 1;
    for (i = 2; i <= s->numTellers; i++)
        if (s->tstat[i].finishService < minfinish) {
            minfinish = s->tstat[i].finishService;
            num[0] = i;
            m = 1;
        }
        else {
            if (s->tstat[i].finishService == minfinish)
                num[m++] = i;
        }
    //1号窗口是vip窗口，如果是vip则返回优先级：空vip窗口1>空普通窗口>非空vip窗口>非空普通窗口
    if (status == Vip) {
        if (s->tstat[1].finishService == 0) return 1;
        else {
            if (m == 1 && num[0]!=1) return 1;
            else {
                return 1 + rand() % (s->numTellers - 1);
            }
        }
    }
    //如果不是vip，不能使用1号窗口
    else {
        if (num[0]==1){
            if(s->tstat[1].finishService== 0 && m==1) return 1 + rand() % (s->numTellers - 1);
            else if(s->tstat[1].finishService== 0 && m!=1) return num[1+rand() % (m-1)];
            else return 1 + rand() % (s->numTellers - 1);
        }
        else{
            if(s->tstat[num[0]].finishService== 0 && m!=1) return num[rand() % m];
            else return 1 + rand() % (s->numTellers - 1);
            
        }
    }
}

/* 运行事件驱动模拟 */
void RunSimulation(Simulation* s)
{
    Event* e = (Event*)malloc(sizeof(Event));
    Event* newevent = (Event*)malloc(sizeof(Event));
    int nexttime, tellerID, servicetime, waittime;
    isVip customertype,precustomertype;
    /* 遍历事件优先级队列，直到队列为空 */
    while (!PQEmpty(&(s->pq)))
    {
        /* 获得下一个事件的优先级，此处指的是时间产生时间time */
        *e = PQDelete(&(s->pq));

        if (e->etype == arrival){
            /* 处理到达事件 */
            nexttime = e->time + NextArrivalTime(s);
            customertype = (rand() % 4 == 0) ? Vip : notVip;
            if (nexttime <= s->simulationLength)
                /* 如果到达事件在下班时刻之后，则不处理到达事件 */
            {
                /* 生成下一客户到达事件并加入事件优先级队列 */
                s->nextCustomer++;
                //printf("%d\n",s->nextCustomer);
                InitEvent(newevent, nexttime, arrival, s->nextCustomer, 0, 0, 0, customertype);
                PQInsert(&(s->pq), *newevent);
            }
            if(e->isvip == Vip) printf("Time: %2d\tarrival of customer %d(Vip)\n", e->time, e->customerID);
            else printf("Time: %2d\tarrival of customer %d(normal)\n", e->time, e->customerID);
            //printf("%d*********\n",PQEmpty(&(s->pq)));

            /* 生成到达事件的离开事件 */
            servicetime = Get_ServiceTime(s);
            /* 计算可供服务的出纳窗口 */
            tellerID = NextAvailableTeller(s,e->isvip);
            
            /* 如果选定的出纳窗口是空闲的，则更新完成时间预告为当前到达事件时间，以便后续更新完成时间 */
            if (s->tstat[tellerID].finishService == 0)
            {
                 s->tstat[tellerID].finishService = e->time;
            }

            /* 计算等待时间 */
            waittime = s->tstat[tellerID].finishService - e->time;

            /* 按照当前到达事件信息修改选定出纳窗口信息 */
            s->tstat[tellerID].totalCustomerWait += waittime;
            s->tstat[tellerID].totalCustomerCount++;
            s->tstat[tellerID].totalService += servicetime;
            s->tstat[tellerID].finishService += servicetime;

            /* 将生成的离开事件加入优先级队列 */
            InitEvent(newevent, s->tstat[tellerID].finishService,
                departure, e->customerID, tellerID, waittime, servicetime, e->isvip);

            PQInsert(&(s->pq), *newevent);
        }
        /* 处理离开事件 */
        else
        {
            if(e->isvip == Vip) printf("Time: %2d\tdeparture of customer %d(Vip)\n", e->time, e->customerID);
            else printf("Time: %2d\tdeparture of customer %d(normal)\n", e->time, e->customerID);
            printf("\tTeller %d\tWait %d\tService %d\n", e->tellerID, e->waittime, e->servicetime);
            tellerID =e->tellerID;
            /* 如果窗口在处理离开事件后空闲，则修改该出纳窗口的空闲时间预告 */
            if (e->time == s->tstat[tellerID].finishService)
                s->tstat[tellerID].finishService = 0;
        }
    }

    /* 修改总的服务时间，注意可能受离开事件影响导致实际总的服务时间大于预定的服务时间 */
    s->simulationLength = (e->time <= s->simulationLength)
        ? s->simulationLength : e->time;
}

/* 输出模拟结果 */
void PrintSimulationResults(Simulation* s)
{
    int cumCustomers = 0, cumWait = 0, i;
    int avgCustWait, tellerWorkPercent;
    float tellerWork;

    for (i = 1; i <= s->numTellers; i++)
    {
        cumCustomers += s->tstat[i].totalCustomerCount; 
        cumWait += s->tstat[i].totalCustomerWait;
    }

    printf("\n******** Simulation Summary ********\n");
    printf("Simulation of %d minutes\n", s->simulationLength);
    printf("\tNo. of Customers: %d\n", cumCustomers);   
    printf("\tAverage Customer Wait: ");

    avgCustWait = (int)((float)cumWait / cumCustomers + 0.5);
    printf("%d minutes\n", avgCustWait);
    for (i = 1; i <= s->numTellers; i++)
    {
        printf("\tTeller #%d\tWorking ", i);
        /* 显示每个出纳窗口的服务时间占比 */
        tellerWork = (float)(s->tstat[i].totalService) / s->simulationLength;
        tellerWorkPercent = (int)(tellerWork * 100.0 + 0.5);
        printf("%d%%\n", tellerWorkPercent);
    }
}

#endif  /* SIMULATION */