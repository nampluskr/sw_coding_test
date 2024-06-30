#if 0
#define MAX_HEAP 1000000
#define MAX_SIZE 1000000
#define MAX_ROBOT 50010
#define MAX_WORK 50010
#define MAX_WORKTIME 1000000000

struct Data {
 int id;
 int worktime;
 unsigned long long value;
};

struct MinHeap {
 Data heap[MAX_SIZE];

 int heapSize = 0;

 void heapInit(void)
 {
  heapSize = 0;
 }

 void heapPush(int id, int worktime, unsigned long long value)
 {
  heap[heapSize].id = id;
  heap[heapSize].worktime = worktime;
  heap[heapSize].value = value;

  int current = heapSize;
  while (current > 0 && heap[current].value < heap[(current - 1) / 2].value)
  {
   Data temp = heap[(current - 1) / 2];
   heap[(current - 1) / 2] = heap[current];
   heap[current] = temp;
   current = (current - 1) / 2;
  }

  heapSize = heapSize + 1;

  return;
 }

 Data heapPop()
 {
  Data ret = heap[0];
  heapSize = heapSize - 1;

  heap[0] = heap[heapSize];

  int current = 0;
  while (current * 2 + 1 < heapSize)
  {
   int child;
   if (current * 2 + 2 == heapSize)
   {
    child = current * 2 + 1;
   }
   else
   {
    child = heap[current * 2 + 1].value < heap[current * 2 + 2].value ? current * 2 + 1 : current * 2 + 2;
   }

   if (heap[current].value < heap[child].value)
   {
    break;
   }

   Data temp = heap[current];
   heap[current] = heap[child];
   heap[child] = temp;

   current = child;
  }
  return ret;
 }


};

struct MaxHeap {
 Data heap[MAX_SIZE];

 int heapSize = 0;

 void heapInit(void)
 {
  heapSize = 0;
 }

 void heapPush(int id, int worktime, unsigned long long value)
 {
  heap[heapSize].id = id;
  heap[heapSize].worktime = worktime;
  heap[heapSize].value = value;

  int current = heapSize;
  while (current > 0 && heap[current].value > heap[(current - 1) / 2].value)
  {
   Data temp = heap[(current - 1) / 2];
   heap[(current - 1) / 2] = heap[current];
   heap[current] = temp;
   current = (current - 1) / 2;
  }

  heapSize = heapSize + 1;

  return;
 }

 Data heapPop()
 {
  Data ret = heap[0];
  heapSize = heapSize - 1;

  heap[0] = heap[heapSize];

  int current = 0;
  while (current * 2 + 1 < heapSize)
  {
   int child;
   if (current * 2 + 2 == heapSize)
   {
    child = current * 2 + 1;
   }
   else
   {
    child = heap[current * 2 + 1].value > heap[current * 2 + 2].value ? current * 2 + 1 : current * 2 + 2;
   }

   if (heap[current].value > heap[child].value)
   {
    break;
   }

   Data temp = heap[current];
   heap[current] = heap[child];
   heap[child] = temp;

   current = child;
  }
  return ret;
 }


};

struct ListNode {
 int idx;
 ListNode* next;
};

int hrp;
ListNode heap[MAX_HEAP];
ListNode* head[MAX_WORK];

ListNode* appendListNode(int idx, ListNode* head)
{
 ListNode* node = &heap[hrp++];
 node->idx = idx;
 node->next = head;

 return node;
}

MaxHeap highIQ;
MinHeap lowIQ;

struct Robot {
 int lastCallTime;      // 마지막으로 일하러 간 시간,
 int wID;               // wID가 0이면 고장, -1이면 트레이닝센터
 int worktime;          // 일하러 간 시간의 총합 //IQ = ctime - worktime
}robot[MAX_ROBOT];

int n; //로봇의 수

//계속 변하는 값을 우큐에 넣으면 안된다 (이 문제의 중요한 포인트)
//IQ는 트레이닝 센터에 있는 한 계속 증가한다
//IQ(변경) = ctime(변경) - sum(worktime)(트레이닝센터에 있는 애들 한해 고정)

//우큐에서 뽑아내야 하는 대상은 트레이닝 센터에 있는 애들
//우큐에 값이 들어가야 하는 상황은 고장난 애들 또는 일하고 복귀한 애들

/////////////////////////////////////////////////////////////////////
void init(int N)
{
    highIQ.heapInit();
    lowIQ.heapInit();
    hrp = 0;
    n = N;
    unsigned long long value = 0;

    for (int i = 0; i < MAX_WORK; i++) { head[i] = 0; }
    for (int i = 1; i <= n; i++) {
        robot[i].worktime = 0;
        robot[i].wID = -1;

        value = (MAX_WORKTIME - robot[i].worktime) * (unsigned long long)MAX_ROBOT + (unsigned long long)(MAX_ROBOT - i);
        highIQ.heapPush(i, robot[i].worktime, value);

        value = (MAX_WORKTIME - robot[i].worktime) * (unsigned long long)MAX_ROBOT + (unsigned long long)i;
        lowIQ.heapPush(i, robot[i].worktime, value);
    }
}

int callJob(int cTime, int wID, int mNum, int mOpt)
{
    int inputRobots = 0;
    Data data;
    int ret = 0;

    while (inputRobots < mNum && highIQ.heapSize > 0 && lowIQ.heapSize > 0) {
        if (mOpt == 0) data = highIQ.heapPop();
        if (mOpt == 1) data = lowIQ.heapPop();

        //팝데이터의 정합성이 일치하면 로봇을 wid에 할당
        if (robot[data.id].wID == -1 && robot[data.id].worktime == data.worktime) {
            robot[data.id].wID = wID;
            robot[data.id].lastCallTime = cTime;
            head[wID] = appendListNode(data.id, head[wID]);
            ret += data.id;
            inputRobots++;
        }
    }
    return ret;
}

void returnJob(int cTime, int wID)
{
    ListNode* temp = head[wID];
    unsigned long long value = 0;

    while (temp != 0) {
        if (robot[temp->idx].wID == wID && highIQ.heapSize < MAX_SIZE && lowIQ.heapSize < MAX_SIZE) {
            robot[temp->idx].worktime += (cTime - robot[temp->idx].lastCallTime);
            robot[temp->idx].wID = -1;

            value = (MAX_WORKTIME - robot[temp->idx].worktime) * (unsigned long long)MAX_ROBOT + (unsigned long long)(MAX_ROBOT - temp->idx);
            highIQ.heapPush(temp->idx, robot[temp->idx].worktime, value);

            value = (MAX_WORKTIME - robot[temp->idx].worktime) * (unsigned long long)MAX_ROBOT + (unsigned long long)temp->idx;
            lowIQ.heapPush(temp->idx, robot[temp->idx].worktime, value);
        }
        temp = temp->next;
    }
}

void broken(int cTime, int rID)
{
    if (robot[rID].wID > 0)
        robot[rID].wID = 0;
}

void repair(int cTime, int rID)
{
    unsigned long long value = 0;

    if (robot[rID].wID == 0)
    {
        robot[rID].worktime = cTime;
        robot[rID].wID = -1;

        value = (MAX_WORKTIME - robot[rID].worktime) * (unsigned long long)MAX_ROBOT + (unsigned long long)(MAX_ROBOT - rID);
        highIQ.heapPush(rID, robot[rID].worktime, value);

        value = (MAX_WORKTIME - robot[rID].worktime) * (unsigned long long)MAX_ROBOT + (unsigned long long)rID;
        lowIQ.heapPush(rID, robot[rID].worktime, value);
 }
}

int check(int cTime, int rID)
{
    if (robot[rID].wID == 0)
        return 0;
    else if (robot[rID].wID > 0)
        return robot[rID].wID * -1;
    else
        return cTime - robot[rID].worktime;
}
#endif