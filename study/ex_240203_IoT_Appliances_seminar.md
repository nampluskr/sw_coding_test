### []

```cpp
#define ABS(x) (((x) > 0) ? (x) : -(x))
const int MAX_DEVICE = 256;
const int MAX_ROOM_INDEX = 256;
const int MAX_ROOM = 52;

int totalDeviceCount;
int detectedDeviceCount;

struct DetectedDevice
{
    int id;
    int power;
    int tx;
    int ty;
};

extern int scan_device(int mDeviceId, int mScanPower, DetectedDevice mDetected[]);

struct Device
{
    int id;
    int roomIndex;
    int scanPower;
    int detectedDeviceCount;
    DetectedDevice detectedDevice[MAX_DEVICE];
    
    void scanPowerUp(int power)
    {
       scanPower += power;
    }

    void scan()
    {
       detectedDeviceCount = scan_device(id, scanPower, detectedDevice);
    }
};

Device device[MAX_DEVICE];

struct Data
{
    int key, value;
};

struct PriorityQueue
{
    Data heap[10000];
    int heapSize = 0;
 
    void heapInit()
    {
       heapSize = 0;
    }
 
    void heapPush(int key, int value)
    {
       heap[heapSize].key = key;
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
    }
 
    Data heapPop()
    {
       Data value = heap[0];
       heapSize = heapSize - 1;
       heap[0] = heap[heapSize];
 
       int current = 0;
       while (current * 2 + 1 < heapSize)
       {
          int child;
          if (current * 2 + 2 == heapSize)
             child = current * 2 + 1;
          else
             child = heap[current * 2 + 1].value < heap[current * 2 + 2].value ? current * 2 + 1 : current * 2 + 2;
 
          if (heap[current].value < heap[child].value)
             break ;
          
          Data temp = heap[current];
          heap[current] = heap[child];
          heap[child] = temp;
          current = child;
       }
       return value;
    }
};

PriorityQueue deviceByRoom[MAX_ROOM_INDEX];
PriorityQueue room;

int newDevice(int mDeviceId)
{
    device[detectedDeviceCount].id = mDeviceId;
    device[detectedDeviceCount].roomIndex = detectedDeviceCount;
    device[detectedDeviceCount].scanPower = 0;
    device[detectedDeviceCount].detectedDeviceCount = 0;
    return detectedDeviceCount++;
}

int getDeviceIndex(int deviceId)
{
    for (int i = 0; i < detectedDeviceCount; ++i)
    {
       if(device[i].id == deviceId)
          return i;
    }
    return newDevice(deviceId);
}

int getLowestScanPowerDeviceIndex()
{
    int minScanPower = 0x7fffffff;
    int deviceId = -1;
    for (int i = 0; i < detectedDeviceCount; ++i)
    {
       if(device[i].scanPower >= minScanPower)
          continue;
       deviceId = i;
       minScanPower = device[i].scanPower;
    }
    return deviceId;
}

void floodFill(int originIndex, int newRoomIndex)
{
    for (int i = 0; i < detectedDeviceCount; ++i)
    {
       if(device[i].roomIndex == originIndex)
          device[i].roomIndex = newRoomIndex;
    }
}

void updateRoomIndexOfDetectedDevice(int scanDeviceId)
{
    for (int i = 0; i < device[scanDeviceId].detectedDeviceCount; ++i)
    {
       DetectedDevice detectedDevice = device[scanDeviceId].detectedDevice[i];
       int index = getDeviceIndex(detectedDevice.id);
       int power = device[scanDeviceId].scanPower - detectedDevice.power; // 소모된 power를 구한다. 

       if(power != ABS(detectedDevice.tx) + ABS(detectedDevice.ty)) // 같은 방에 있다고 확정할 수 없다면 continue
          continue;

       if(device[index].roomIndex == index) // 고독한(방에 혼자있는) device라면
          device[index].roomIndex = device[scanDeviceId].roomIndex; // 그 device의 roomIndex만 바꾸면되고
       else
          floodFill(device[index].roomIndex, device[scanDeviceId].roomIndex); // 그게 아니면 그 친구들까지도 모두 바꿔줘야 한다.
    }
}

void detectAllDevice()
{
    while(detectedDeviceCount < totalDeviceCount)
    {
       // scan : scan을 시킬 device를 누구로 정할것인가?
       // 상대위치를 아니까 즉 최초 알려준 장치의 위치를 100,100으로 가정하고 같은 방에 있는 장치는 tx,ty값을 신뢰할수 있으니까 상대 좌표를 구할수 있다. 그 좌표를 scan 할 장치 선정에 활용하면
       // 조~~~~~~~~~~~~오금은 개선될 수 있을 것이나 들이는 시간대비 효율은 안좋을 것이다.
       // 그리고 얼마의 power로 scan을 할 것인가? : 점수는 power의 세제곱으로 증가되니까 한번에 크게 하기보단 조금씩 증가시키면서 넓혀나가는 방식이 좋겠다.
       // 그럼 중분은 어떻게 가져갈 것인가? : 크게 영향을 주지는 않는다.
       
       const int idx = getLowestScanPowerDeviceIndex();
       device[idx].scanPowerUp(200);
       device[idx].scan();
       updateRoomIndexOfDetectedDevice(idx);
    }
}

void findDevice()
{
    for (int i = 0; i < totalDeviceCount; ++i)
    {
       if(device[i].scanPower >= 200)
          continue;
       //if(device[i].roomIndex != i )
       // continue;
       device[i].scanPower = 200;
       device[i].scan();
       updateRoomIndexOfDetectedDevice(i);
    }
}

void scan(int mDeviceId, int mTotalDevice)
{
    totalDeviceCount = mTotalDevice;
    detectedDeviceCount = 0;
    newDevice(mDeviceId);
    // 문제가 복잡하니까. 확실한 것부터 생각을 해보시면 좋습니다.
    // 분류를 해야한다. 일단 뭐가 있는지를 알아야 한다.
    // 일단 어떤 device들이 있는지 찾아야한다.
    // 그 후에 분류는 결국 개별방에 있다고 설정해둔 device들을 방 별로 merge하는 것이다.
    detectAllDevice(); 
    findDevice();
}

void result(int mDeviceIds[][MAX_DEVICE])
{
    for (int i = 0; i < MAX_ROOM_INDEX; ++i)
    {
       deviceByRoom[i].heapInit();
    }

    for (int i = 0; i < totalDeviceCount; ++i)
    {
       deviceByRoom[device[i].roomIndex].heapPush(device[i].id,device[i].id);
    }

    room.heapInit();
    for (int i = 0; i < MAX_ROOM_INDEX; ++i)
    {
       if(deviceByRoom[i].heapSize == 0) // i번째 방은 다른 방으로 흡수 합병된 방이란 뜻이 된다.
          continue;
       room.heapPush(i, deviceByRoom[i].heap[0].value);
    }

    int roomCount = room.heapSize;

    for (int i = 0; i < roomCount; ++i)
    {
       const Data rm = room.heapPop();
       int heapsize = deviceByRoom[rm.key].heapSize;

       for (int j = 0; j < heapsize; ++j)
       {
          int deviceId = deviceByRoom[rm.key].heapPop().key;
          mDeviceIds[i][j] = deviceId;
       }
    }
}
```
