/* 事件类型定义 */
enum eventType { arrival, departure };
typedef enum eventType EventType;
enum isVip {notVip, Vip};
struct event
{
	int time;
	EventType etype;
	int customerID;     
	int tellerID;     
	int waittime;
	int servicetime;
	isVip isvip;
};
typedef struct event Event;

/* 事件初始化函数 */
void InitEvent(Event* e, int t, EventType et, int cn, int tn, int wt, int st, isVip customertype)
{
	e->time = t;
	e->etype = et;
	e->customerID = cn;
	e->tellerID = tn;
	e->waittime = wt;
	e->servicetime = st;
	e->isvip = customertype;
}

/* 比较事件时间为事件优先队列中的优先级，时间相同则离开事件优先 */
int CompareEvent(Event* e1, Event* e2)
{
	if (e1->time < e2->time || 
		(e1->time == e2->time && e1->etype == departure)) 
		return 1; // e1优先
	else if (e1->time > e2->time ||
		(e1->time == e2->time && e2->etype == departure))
		return 0; // e2优先
	return 0; // 默认返回值
}