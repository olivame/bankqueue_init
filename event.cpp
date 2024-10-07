/* 以下均为 event.c 内容 */
#include "sim.h"
#include<time.h>

int main(void)
{
    /* 生命模拟结构的存储空间 */
    Simulation* S = (Simulation*)malloc(sizeof(Simulation));
    srand(time(NULL));  /* 设置随机数种子 */
    InitSimulation(S);

    /* 运行事件驱动模拟 */
    RunSimulation(S);

    /* 输出模拟结果 */
    PrintSimulationResults(S);
    return 0;
}