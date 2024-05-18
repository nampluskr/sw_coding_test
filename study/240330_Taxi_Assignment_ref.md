### [박영규] 240330_Taxi Assignment

- Score가 가장 낮은 Driver를 찾아 해당 Driver에 가장 가까운 승객을 할당 (Greedy)
- 모든 승객을 할당 후 Score가 가장 높은 Driver에 할당된 승객을 점수가 낮은 Driver들(하위 25명) 에게 재할당 했을 때
- 점수가 가장 적게 늘어나는 승객, Driver를 찾아 재할당 (TC당 MAX 65회)
- Score : 429'984'294 (9.302s)

```cpp
#define ABS(x)((x) > 0 ? (x) : -(x))
#define DRIVER_NUM (100)
#define PASSENGER_NUM (10000)
#define WIDTH (25)
#define REARRANGECNT (65)

struct Coordinate {
	int y, x;
};

struct Passenger {
	Coordinate departure;
	Coordinate arrival;
};

struct PassengerInfo {
	Passenger info;
	int driverId;
};

struct DriverInfo {
	Coordinate pos;
	int passengerList[PASSENGER_NUM];
	int scoreList[PASSENGER_NUM];
	int pCnt;
	long long score;
};

DriverInfo driverInfo[DRIVER_NUM];
PassengerInfo passengerInfo[PASSENGER_NUM];

int numDriver;
int numPassenger;
int sortedDriverList[DRIVER_NUM];

extern bool assign_driver(int driverID, int passengerSize, int passengerIDs[]);

void quickSort(int first, int last)
{
	int pivot;
	int i;
	int j;
	int temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (driverInfo[sortedDriverList[i]].score <= driverInfo[sortedDriverList[pivot]].score && i < last)
			{
				i++;
			}
			while (driverInfo[sortedDriverList[j]].score > driverInfo[sortedDriverList[pivot]].score)
			{
				j--;
			}
			if (i < j)
			{
				temp = sortedDriverList[i];
				sortedDriverList[i] = sortedDriverList[j];
				sortedDriverList[j] = temp;
			}
		}
		temp = sortedDriverList[pivot];
		sortedDriverList[pivot] = sortedDriverList[j];
		sortedDriverList[j] = temp;
		quickSort(first, j - 1);
		quickSort(j + 1, last);
	}
}

void init()
{
	for (int i = 0; i < numDriver; i++) {
		driverInfo[i].pCnt = 0;
		driverInfo[i].score = 0;
	}

	for (int i = 0; i < numPassenger; i++) 
		passengerInfo[i].driverId = -1;
}

int getMinScoreDriverId()
{
	long long minScore = driverInfo[0].score;
	int ret = 0;

	for (int i = 0; i < numDriver; i++) {
		if (driverInfo[i].score < minScore) {
			minScore = driverInfo[i].score;
			ret = i;
		}
	}
	return ret;
}

int getPassengerId(int dId)
{
	long long minScore = 100000000;
	int ret = 0;

	for (int i = 0; i < numPassenger; i++) {
		if (passengerInfo[i].driverId != -1)
			continue;

		long long score = ABS(passengerInfo[i].info.departure.y - driverInfo[dId].pos.y)
			                         + ABS(passengerInfo[i].info.departure.x - driverInfo[dId].pos.x);
		if (score < minScore) {
			minScore = score;
			ret = i;
		}
	}
	return ret;
}

bool searchSwap()
{
	for (int i = 0; i < numDriver; i++)
		sortedDriverList[i] = i;

	quickSort(0, numDriver - 1);
	int fromDId = sortedDriverList[numDriver - 1];
	int fromIdx = -1, toIdx = -1, swapPId = -1, dId, bestDId;
	long long fromScore, toScore, bestFromScore, bestToScore;
	long long bestScore = 1e15;

	for (int i = 1; i < driverInfo[fromDId].pCnt - 1; i++) {
		int pId = driverInfo[fromDId].passengerList[i];
		int fromDIdPrevId = driverInfo[fromDId].passengerList[i - 1];
		int fromDIdNextId = driverInfo[fromDId].passengerList[i + 1];
		int minus1 = ABS(passengerInfo[pId].info.departure.y - passengerInfo[fromDIdPrevId].info.arrival.y)
			               + ABS(passengerInfo[pId].info.departure.x - passengerInfo[fromDIdPrevId].info.arrival.x)
			               + ABS(passengerInfo[pId].info.arrival.y - passengerInfo[pId].info.departure.y)
			               + ABS(passengerInfo[pId].info.arrival.x - passengerInfo[pId].info.departure.x);

		int minus2 = ABS(passengerInfo[fromDIdNextId].info.departure.y - passengerInfo[pId].info.arrival.y)
		    	               + ABS(passengerInfo[fromDIdNextId].info.departure.x - passengerInfo[pId].info.arrival.x);

		int fromDIdPlus = ABS(passengerInfo[fromDIdNextId].info.departure.y - passengerInfo[fromDIdPrevId].info.arrival.y)
			                          + ABS(passengerInfo[fromDIdNextId].info.departure.x - passengerInfo[fromDIdPrevId].info.arrival.x);

		fromScore = driverInfo[fromDId].score - minus1 - minus2 + fromDIdPlus;
		for (int n = 0; n < WIDTH; n++) {
			if (fromDId == sortedDriverList[n])
				continue;
			dId = sortedDriverList[n];

			for (int k = 1; k < driverInfo[dId].pCnt - 1; k++) {
				int toPrevId = driverInfo[dId].passengerList[k - 1];
				int toNextId = driverInfo[dId].passengerList[k];

				int plus1 = ABS(passengerInfo[pId].info.departure.y - passengerInfo[toPrevId].info.arrival.y)
					           + ABS(passengerInfo[pId].info.departure.x - passengerInfo[toPrevId].info.arrival.x)
					           + ABS(passengerInfo[pId].info.arrival.y - passengerInfo[pId].info.departure.y)
					           + ABS(passengerInfo[pId].info.arrival.x - passengerInfo[pId].info.departure.x);

				int plus2 = ABS(passengerInfo[toNextId].info.departure.y - passengerInfo[pId].info.arrival.y)
					           + ABS(passengerInfo[toNextId].info.departure.x - passengerInfo[pId].info.arrival.x);

				int minus = ABS(passengerInfo[toPrevId].info.arrival.y - passengerInfo[toNextId].info.departure.y)
					             + ABS(passengerInfo[toPrevId].info.arrival.x - passengerInfo[toNextId].info.departure.x);

				if (driverInfo[dId].score + plus1 + plus2 - minus < driverInfo[fromDId].score) {
					toScore = driverInfo[dId].score + plus1 + plus2 - minus;
					long long score = fromScore > toScore ? fromScore : toScore;

					if (score < bestScore) {
						bestScore = score;
						fromIdx = i;
						toIdx = k;
						bestFromScore = fromScore;
						bestToScore = toScore;
						swapPId = pId;
						bestDId = dId;
					}
				}
			}
		}
	}
	if (toIdx == -1)
		return false;

	// toDriver 승객리스트에 삽입
	for (int m = driverInfo[bestDId].pCnt - 1; m >= toIdx; m--) {
		driverInfo[bestDId].passengerList[m + 1] = driverInfo[bestDId].passengerList[m];
	}
	driverInfo[bestDId].passengerList[toIdx] = swapPId;
	driverInfo[bestDId].pCnt++;

	// toDriver 거리 +
	driverInfo[bestDId].score = bestToScore;

	// fromDriver 거리 -
	driverInfo[fromDId].score = bestFromScore;

	// fromDirver 승객리스트에서 삭제
	for (int n = fromIdx; n < driverInfo[fromDId].pCnt - 1; n++)
		driverInfo[fromDId].passengerList[n] = driverInfo[fromDId].passengerList[n + 1];
	driverInfo[fromDId].pCnt--;

	return true;
}

void run(int N, int M, Coordinate mDriver[], int k, Passenger mPassenger[]) 
{
	numDriver = M;
	numPassenger = k;
	init();

	for (int i = 0; i < numDriver; i++) 
		driverInfo[i].pos = mDriver[i];

	for (int i = 0; i < numPassenger; i++)
		passengerInfo[i].info = mPassenger[i];

	for (int i = 0; i < numPassenger; i++) {
		int dId = getMinScoreDriverId(); 
		int pId = getPassengerId(dId);   
		int& wp = driverInfo[dId].pCnt;

		int score = ABS(passengerInfo[pId].info.departure.y - driverInfo[dId].pos.y)
			+ ABS(passengerInfo[pId].info.departure.x - driverInfo[dId].pos.x)
			+ ABS(passengerInfo[pId].info.arrival.y - passengerInfo[pId].info.departure.y)
			+ ABS(passengerInfo[pId].info.arrival.x - passengerInfo[pId].info.departure.x);

		driverInfo[dId].passengerList[wp] = pId;
		driverInfo[dId].scoreList[wp++] = score;
		driverInfo[dId].pos = passengerInfo[pId].info.arrival;
		driverInfo[dId].score += score;
		passengerInfo[pId].driverId = dId;
	}

	for (int i = 0; i < REARRANGECNT; i++) {
		if (!searchSwap())
			break;
	}
	for (int i = 0; i < DRIVER_NUM; i++) {
		assign_driver(i, driverInfo[i].pCnt, driverInfo[i].passengerList);
	}
}
```
