### [김민재]

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

### [리차드]

- SCORE: 1693577795 [~17억 (16.9358)]
- 교육에서 설명한 대로 각 클러스터 내의 상대 위치를 tx, ty에 저장됩니다.
- Plan A: 클러스터에 할당되지 않은 장치를 스캔합니다.
- Plan B: 클러스터의 모든 검색된 장치에서 가장 멀리 있는 검색되지 않은 장치를 찾고 스캔합니다.
- Plan C: 발견되지 않은 장치가 있으면 당황하고 검색 범위를 늘리십시오.

```cpp
#if 0
// tuneable parameters to control scan range and reduce score
const int scan_power_initial = 78;
const int scan_power_new_cluster = 67;
const int scan_power_within_cluster = 170; // why so high?
const int scan_power_panic = 290;
const int scan_power_panic_inc = 300;

#include <stdio.h>
#include <stdlib.h>

template <class T>
void swap(T& a, T& b) {
    T c = a; a = b; b = c;
}

template <typename T>
T min(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

#define ABS(x) (((x) > 0) ? (x) : -(x))

const int MAX_DEVICE = 256;
//const int MAX_ROOM = 52;

struct DetectedDevice
{
    int id, power, tx, ty;
};

extern int scan_device(int mDeviceId, int mScanPower, DetectedDevice mDetected[]);

struct Device
{
    int id;
    int scan_power; // power if scanned. If unscanned: -1 room known, -2 unknown
    int power;      // unused, stores power returned by scan
    int tx;         // x offset within cluster relative to parent
    int ty;         // y offset within cluster relative to parent
    int parent;     // index of parent device within devices array
};

DetectedDevice detected_devices[MAX_DEVICE];
Device devices[MAX_DEVICE];
int num_devices = 0;

// check if devices are in the same room, risky? only guaranteed if scan_power large enough.
// seemingly only a simple test is required/expected, despite the contrived calcuate_power

bool is_same_room(const DetectedDevice& device, int scan_power) {
    return scan_power - device.power == ABS(device.tx) + ABS(device.ty)
        && ABS(device.tx) + ABS(device.ty) != scan_power;
}

// could try to be clever and use the following equations, but likely not worth the effort
// if scan and detected devices separated by 1 horizontal wall then:
// tx_actual = (scan_power - ty + 2*tx - 2*power)/3 (rounding issues!); ty_actual = ty
// if scan and detected devices separated by 1 vertical wall then:
// tx_actual = tx; ty_actual = (scan_power + 2*ty - 2*tx - 2*power)/3 (rounding issues!)
// could use multiple scans from same device to determine number and orientation of walls

int find_device(int device_id) {
    for (int ind_device = 0; ind_device < num_devices; ind_device++)
        if (devices[ind_device].id == device_id)
            return ind_device;
    return -1;
}

void scan(int scan_device_id_initial, int num_devices_total)
{
    num_devices = 0;
    int panic_count = 0;
    int scan_power = scan_power_initial;
    int scan_device_id = scan_device_id_initial;

    while (scan_device_id != -1) {
        int scan_ind = find_device(scan_device_id);
        // create new device and room if not scanned before, scan_device_id must be valid

        if (scan_ind == -1) {
            scan_ind = num_devices;
            devices[scan_ind].id = scan_device_id;
            devices[scan_ind].tx = devices[scan_ind].ty = 0;
            devices[scan_ind].parent = scan_ind;
            num_devices++;
        }
        devices[scan_ind].scan_power = scan_power;
        int parent_ind = devices[scan_ind].parent;
        int num_detected = scan_device(scan_device_id, scan_power, detected_devices);

        if (num_detected == -1) {
            printf("Invalid scan id = %d\n", scan_device_id);
            exit(1);
        }

        // see if scan joins any rooms, find min parent id amongst scanned rooms, fix tx & ty (not easy)
        bool join_room[MAX_DEVICE] = { 0 };
        DetectedDevice offsets[MAX_DEVICE]; // cluster offsets w.r.t. scan_id's cluster (parent->parent)
        int min_parent_ind = devices[scan_ind].parent;
        offsets[min_parent_ind].tx = offsets[min_parent_ind].ty = 0;
        join_room[min_parent_ind] = true;

        for (int id = 0; id < num_detected; id++) {

            const DetectedDevice& device = detected_devices[id];
            int ind_device = find_device(device.id);

            if (ind_device != -1 && devices[ind_device].parent != parent_ind
                && is_same_room(device, scan_power)) {

                int tx = devices[scan_ind].tx + device.tx - devices[ind_device].tx;
                int ty = devices[scan_ind].ty + device.ty - devices[ind_device].ty;

                offsets[devices[ind_device].parent].tx = tx;
                offsets[devices[ind_device].parent].ty = ty;
                join_room[devices[ind_device].parent] = true;

                if (devices[ind_device].parent < min_parent_ind)
                    min_parent_ind = devices[ind_device].parent;
            }
        }

        // add if detected for the first time
        for (int id = 0; id < num_detected; id++) {
            const DetectedDevice& device = detected_devices[id];
            if (find_device(device.id) != -1)
                continue;

            devices[num_devices].id = device.id;
            devices[num_devices].power = device.power;

            if (is_same_room(device, scan_power)) {
                devices[num_devices].tx = devices[scan_ind].tx + device.tx; // add parent offset
                devices[num_devices].ty = devices[scan_ind].ty + device.ty;
                devices[num_devices].scan_power = -1; // room is the same, but not scanned yet
                devices[num_devices].parent = devices[scan_ind].parent; // code below might correct tx, ty!
            }
            else { // new room
                devices[num_devices].tx = devices[num_devices].ty = 0;
                devices[num_devices].scan_power = -2; // not sure about room yet
                devices[num_devices].parent = num_devices;
            }
            num_devices++;
        }

        // join rooms together, modifying all devices in these rooms, update offsets
        for (int ind_device = 0; ind_device < MAX_DEVICE; ind_device++) {
            if (join_room[devices[ind_device].parent] && devices[ind_device].parent != min_parent_ind) {
                devices[ind_device].tx += offsets[devices[ind_device].parent].tx
                    - offsets[min_parent_ind].tx;
                devices[ind_device].ty += offsets[devices[ind_device].parent].ty
                    - offsets[min_parent_ind].ty;
                devices[ind_device].parent = min_parent_ind;
            }
            if (join_room[devices[ind_device].parent] && devices[ind_device].scan_power == -2)
                devices[ind_device].scan_power = -1;
        }

        // find the next candidate device to scan, plan A: check any newly added devices first
        // to do: if num_devices == num_devices_total, using bbox, break if clusters span enough area

        scan_device_id = -1;

        for (int ind_device = 0; ind_device < num_devices && scan_device_id == -1; ind_device++)
            if (devices[ind_device].scan_power == -2) {
                scan_device_id = devices[ind_device].id;
                scan_power = scan_power_new_cluster;
            }

        // failing that, plan B, try to extend a room, pick device furthest from other scanned points

        if (scan_device_id == -1) {
            int parent_ind = -1;

            for (int ind_device = 0; ind_device < num_devices; ind_device++)
                if (devices[ind_device].scan_power == -1) {
                    scan_device_id = devices[ind_device].id;
                    parent_ind = devices[ind_device].parent;
                    break;
                }

            int num_scanned = 0; // number scanned devices within cluster
            bool scanned_ind[MAX_DEVICE];
            for (int ind_device = 0; ind_device < num_devices; ind_device++)
                if (devices[ind_device].parent == parent_ind && devices[ind_device].scan_power > 0)
                    scanned_ind[num_scanned++] = ind_device;

            int max_dist = 0;
            int bbox[] = { 100000, -1, 100000, -1 }; // cluster bounding box (unused) [l,r,b,t]

            for (int ind_device = 0; ind_device < num_devices; ind_device++) {
                if (devices[ind_device].parent == parent_ind) {
                    bbox[0] = min(bbox[0], devices[ind_device].tx);
                    bbox[1] = max(bbox[1], devices[ind_device].tx);
                    bbox[2] = min(bbox[2], devices[ind_device].ty);
                    bbox[3] = max(bbox[3], devices[ind_device].ty);
                }
                if (devices[ind_device].parent != parent_ind || devices[ind_device].scan_power != -1)
                    continue;

                int dist = 0;

                for (int ind_scanned = 0; ind_scanned < num_scanned; ind_scanned++)
                    dist += ABS(devices[ind_device].tx - devices[scanned_ind[ind_scanned]].tx)
                    + ABS(devices[ind_device].ty - devices[scanned_ind[ind_scanned]].ty);
                if (dist > max_dist)
                    max_dist = dist, scan_device_id = devices[ind_device].id;

                devices[ind_device].scan_power = 0; // risky? don't rescan devices in cluster found previously
            }
            scan_power = scan_power_within_cluster; // to do: reduce using bbox, naive attempt failed
        }

        // disaster, time for plan C, use horribly large scanning scan_power 

        if (scan_device_id == -1 && num_devices < num_devices_total) {
            scan_power = scan_power_panic + panic_count * scan_power_panic_inc;
            scan_device_id = scan_device_id_initial; // to do: improve choice of device to scan
            panic_count++;
        }
    } // end while

    if (num_devices < num_devices_total) {
        printf("Only %d out of %d devices detected\n", num_devices, num_devices_total);
        exit(1);
    }
} // end function scan

template <class T, int MAX_SIZE = 256>
class Heap
{
public:
    Heap() : heap_size(0) {}

    T pop() { // bubble down
        swap(data[0], data[--heap_size]);
        for (int index = 0; (2 * index + 1 < heap_size && data[2 * index + 1] < data[index])
            || (2 * index + 2 < heap_size && data[2 * index + 2] < data[index]); )
            if (2 * index + 2 < heap_size && data[2 * index + 2] < data[2 * index + 1]) {
                swap(data[index], data[2 * index + 2]);
                index = 2 * index + 2;
            }
            else {
                swap(data[index], data[2 * index + 1]);
                index = 2 * index + 1;
            }
        return data[heap_size];
    }

    void push(const T& val) { // bubble up
        data[heap_size++] = val;
        for (int index = heap_size - 1; index != 0
            && data[index] < data[(index - 1) / 2]; index = (index - 1) / 2)
            swap(data[index], data[(index - 1) / 2]);
    }
    int size() {
        return heap_size;
    }
    void clear() {
        heap_size = 0;
    }
private:
    T data[MAX_SIZE];
    int heap_size;
};

struct pair
{
    pair(int first = 0, int second = 0) : first(first), second(second) {}
    bool operator<(const pair& other) {
        return first < other.first;
    }
    int first, second;
};

Heap<pair> qroom;
Heap<int> qdev;

void result(int device_ids[][MAX_DEVICE])
{
    int min_id[MAX_DEVICE]; // parent_ind -> minimum device id in cluster/room
    for (int ind_device = 0; ind_device < MAX_DEVICE; ind_device++)
        min_id[ind_device] = -1;

    for (int ind_device = 0; ind_device < num_devices; ind_device++)
        if (min_id[devices[ind_device].parent] == -1
            || devices[ind_device].id < min_id[devices[ind_device].parent])
            min_id[devices[ind_device].parent] = devices[ind_device].id;

    qroom.clear(); // heap used to sort rooms by id

    for (int parent_ind = 0; parent_ind < num_devices; parent_ind++)
        if (min_id[parent_ind] != -1)
            qroom.push(pair(min_id[parent_ind], parent_ind));

    for (int room_count = 0; qroom.size() != 0; room_count++) {
        int parent_ind = qroom.pop().second;

        qdev.clear();
        for (int ind_device = 0; ind_device < num_devices; ind_device++)
            if (devices[ind_device].parent == parent_ind)
                qdev.push(devices[ind_device].id);

        for (int ind_device = 0; qdev.size() != 0; ind_device++)
            device_ids[room_count][ind_device] = qdev.pop();
    }
} // end function result
#endif
```
