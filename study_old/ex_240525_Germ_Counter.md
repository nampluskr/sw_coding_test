### [main.cpp]

```cpp
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

static unsigned long long seed = 5;
static int pseudo_rand(void)
{
	seed = seed * 25214903917ULL + 11ULL;
	return (seed >> 16) & 0x3fffffff;
}

/* These constant variables will NOT be changed */
static const long long PENALTY = 10'000'000'000'000LL;
static const int MAX_TC = 10;

static const int MAX_CORE = 16384;
static const int CORE_BUF_SIZE = 1024;
static const int MAX_GERM = 1'000'000;
static const int MAX_COLOR = 16;
static const int MAX_GERM_TYPE = 1000;

static long long SCORE;
static long long gCoreCycle[MAX_CORE];
static int gAnswer;

struct Germs {
	int size;
	int color;
	int direction;
	int degree;
	int shape;
	int usedCore;
};

static struct Germs germs[MAX_GERM], target;
static int gGpuBuffer[MAX_CORE][CORE_BUF_SIZE];
static int gGpuBufferIndex[MAX_CORE];

#define ABS(x) (((x) > 0) ? (x) : -(x))

//////////////////////////
void info(int coreIndex, int id)
{
	if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)
		return;
	gCoreCycle[coreIndex] += 1;

	if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {
		germs[id].usedCore = coreIndex;
		int& bufferIndex = gGpuBufferIndex[coreIndex];
		gGpuBuffer[coreIndex][bufferIndex++] = 0; // info
		gGpuBuffer[coreIndex][bufferIndex++] = id;
		gGpuBuffer[coreIndex][bufferIndex++] = germs[id].size;
		gGpuBuffer[coreIndex][bufferIndex++] = germs[id].degree;
		gGpuBuffer[coreIndex][bufferIndex++] = germs[id].color;
	}
}

void rotate(int coreIndex, int id, int degree)
{
	if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)
		return;

	gCoreCycle[coreIndex] += 2;

	if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {
		germs[id].usedCore = coreIndex;
		germs[id].degree = (germs[id].degree + degree) % 360;
	}
}

void scale(int coreIndex, int id, int scale)
{
	if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)
		return;

	gCoreCycle[coreIndex] += 3;

	if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {
		germs[id].usedCore = coreIndex;
		germs[id].size = germs[id].size + scale;
	}
}

void match(int coreIndex, int id)
{
	if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)
		return;

	gCoreCycle[coreIndex] += 5;

	if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {
		germs[id].usedCore = coreIndex;

		int similarity = ABS(target.color - germs[id].color);
		similarity += ABS(target.shape - germs[id].shape);
		similarity += ABS(target.size - germs[id].size);
		int direction = ABS(target.direction - (germs[id].direction + germs[id].degree)) % 360;
		similarity += (direction < 180) ? direction : 360 - direction;

		int& bufferIndex = gGpuBufferIndex[coreIndex];
		gGpuBuffer[coreIndex][bufferIndex++] = 1; // match
		gGpuBuffer[coreIndex][bufferIndex++] = id;
		gGpuBuffer[coreIndex][bufferIndex++] = similarity;
	}
}

void gpu_process(int sharedBuffer[MAX_CORE][CORE_BUF_SIZE])
{
	for (int i = 0; i < MAX_CORE; i++)
		for (int j = 0; j < gGpuBufferIndex[i]; j++)
			sharedBuffer[i][j] = gGpuBuffer[i][j];

	for (int i = 0; i < MAX_CORE; i++)
		gGpuBufferIndex[i] = 0;

	int maxScore = 0;
	for (int coreIndex = 0; coreIndex < MAX_CORE; coreIndex++) {
		if (maxScore < gCoreCycle[coreIndex])
			maxScore = gCoreCycle[coreIndex];
		gCoreCycle[coreIndex] = 0;
	}
	SCORE += (maxScore + 10);
	for (int i = 0; i < MAX_GERM; i++)
		germs[i].usedCore = -1;
}

void result(int answer)
{
	gAnswer = answer;
}

//////////////////////////
static void init()
{
	gAnswer = -1;

	for (int i = 0; i < MAX_GERM; i++) {
		germs[i].size = pseudo_rand() % 10;
		germs[i].direction = pseudo_rand() % 360;
		germs[i].degree = 0;
		germs[i].color = pseudo_rand() % MAX_COLOR;
		germs[i].shape = pseudo_rand() % MAX_GERM_TYPE;
		germs[i].usedCore = -1;
	}
	target.size = pseudo_rand() % 10;
	target.direction = pseudo_rand() % 360;
	target.degree = 0;
	target.color = pseudo_rand() % MAX_COLOR;
	target.shape = pseudo_rand() % MAX_GERM_TYPE;

	for (register int i = 0; i < MAX_CORE; i++)
	{
		gGpuBufferIndex[i] = 0;
		gCoreCycle[i] = 0;
	}
}

static bool verify()
{
	int count = 0;

	for (int i = 0; i < MAX_GERM; i++)
		if (target.color == germs[i].color && target.shape == germs[i].shape)
			count++;

	if (count == gAnswer)
		return true;

	return false;
}

extern void process();

int main()
{
	setbuf(stdout, NULL);
	SCORE = 0;

	for (int tc = 0; tc < MAX_TC; ++tc) {
		init();
		process();

		if (verify() == false) {
			SCORE = PENALTY;
			break;
		}
	}
	printf("SCORE: %lld\n", SCORE);
	return 0;
}
```

### [user.cpp]

```cpp
static const int MAX_CORE = 16384;
static const int MAX_GERM = 1'000'000;
static const int CORE_BUF_SIZE = 1024;

extern void info(int coreIndex, int id);
extern void rotate(int coreIndex, int id, int degree);
extern void scale(int coreIndex, int id, int scale);
extern void match(int coreIndex, int id);
extern void gpu_process(int sharedBuffer[MAX_CORE][CORE_BUF_SIZE]);
extern void result(int anwer);

void process()
{
}
```
### [백승재]

- Score : 4662 (시험제출은 6천점대로 제출했습니다.)
- 전체 match 후에 target_size를 구하고
- direction에 의한 유사도 차이 180 미만인 것만 고려해서 rotate 후 count하였습니다.
- 좀 더 최적화 하려면 (아래 내용은 스포방지 Drag해야 보임)

```cpp
#define MAX_CORE 16'384
#define BUFFER_SIZE 1024
#define MAX_GERM 1'000'000

extern void info(int core_id, int id);
extern void scale(int core_id, int id, int size);
extern void rotate(int core_id, int id, int degree);
extern void match(int core_id, int id);
extern void gpu_process(int shared_buffer[MAX_CORE][BUFFER_SIZE]);
extern void set_answer(int ans);

#define ABS(x) (((x) > 0) ? (x) : -(x))

struct MyGerm {
	int id;
	int size;
	int degree;
	int color;
	int similarity;
};

MyGerm _germ[MAX_GERM];
int _germ2Size;
MyGerm _germ2[MAX_GERM];
int _buffer[MAX_CORE][BUFFER_SIZE];
int _bufferSize[MAX_CORE];
int _targetSize;
int answer;

void init()
{
	for (int i = 0; i < MAX_GERM; i++)
	{
		_germ[i].id = -1;
		_germ[i].size = 0;
		_germ[i].degree = 0;
		_germ[i].color = 0;
		_germ[i].similarity = 0;
		_germ[i].id = -1;
		_germ2[i].size = 0;
		_germ2[i].degree = 0;
		_germ2[i].color = 0;
		_germ2[i].similarity = 0;
	}
	_germ2Size = 0;
	_targetSize = 0;
	answer = 0;
}

void GetSize()
{
	int coreSize = MAX_CORE - 1;

	//첫 타자를 희생해서 Target Size를 알아냄.
	match(0, 0);
	scale(0, 0, 10);
	match(0, 0);
	scale(0, 0, -20);
	match(0, 0);
	info(0, 0);

	int idx = 0;
	for (int i = 1; i < MAX_GERM; i++)
	{
		idx = i % coreSize + 1;
		info(idx, i);
		match(idx, i);
	}
	gpu_process(_buffer);

	for (int i = 0; i < MAX_CORE; i++)
		_bufferSize[i] = 0;

	for (int i = 1; i < MAX_GERM; i++)
	{
		idx = i % coreSize + 1;
		_germ[i].size = _buffer[idx][_bufferSize[idx] + 1];
		_germ[i].degree = _buffer[idx][_bufferSize[idx] + 2];
		_germ[i].color = _buffer[idx][_bufferSize[idx] + 3];

		_bufferSize[idx] += 5;
		_germ[i].similarity = _buffer[idx][_bufferSize[idx] + 1];
		_bufferSize[idx] += 3;
	}

	//완전 초기값
	int a = _buffer[0][1];

	//10 더해진 값
	int b = _buffer[0][3 + 1];

	//-20 더해진 값
	int c = _buffer[0][6 + 1];

	_germ[0].size = _buffer[0][9 + 1];
	_germ[0].degree = _buffer[0][9 + 2];
	_germ[0].color = _buffer[0][9 + 3];
	_germ[0].similarity = c;

	if (b - a == 10)
	{
		if (c - a == 10)
		{
			//targetsize와 0번째 size가 같은 경우
			_targetSize = _germ[0].size + 10;
		}
		else
		{
			//targetsize가 0번째 size보다 작은 경우
			_targetSize = _germ[0].size + (10 - (a - c)) / 2;
		}
	}
	else
	{
		//targetsize가 0번째 size보다 큰 경우
		_targetSize = _germ[0].size + 10 + (10 - (b - a)) / 2;
	}
}

void checkItem()
{
	//찾으려는 세균의 경우 Size와 Direction만 차이가 있어야 가능하므로
	//Size와 Direction의 차이가 가질수 있는 최대값 9+180을 고려해서
	//구해진 Size는 제거하고 180 이하인 항목만 대상으로 걸러냄

	for (int i = 0; i < MAX_GERM; i++)
	{
		if ((_germ[i].similarity - ABS(_targetSize - _germ[i].size)) < 181)
		{
			_germ2[_germ2Size].id = i;
			_germ2[_germ2Size].size = _germ[i].size;
			_germ2[_germ2Size].degree = _germ[i].degree;
			_germ2[_germ2Size].color = _germ[i].color;
			_germ2[_germ2Size].similarity = _germ[i].similarity;
			_germ2Size++;
		}
	}
}

//type 1 : size까지 조정해서 비교 → 5772
//type 2 : 각도를 2번 돌려서 비교 → 5404
//type 3 : 각도를 1번만 돌려서 비교 → 4662

void answer_count(int type)
{
	//answer
	int setCount = 0;
	int idx = 0;
	int value = 0;

	for (int i = 0; i < _germ2Size; i++)
	{
		idx = i % MAX_CORE;
		if (type > 0 && type < 3)
		{
			if (type == 1)
				scale(idx, _germ2[i].id, _targetSize - _germ2[i].size);

			value = _germ2[i].similarity - ABS(_targetSize - _germ2[i].size);
			rotate(idx, _germ2[i].id, value);
			match(idx, _germ2[i].id);
			rotate(idx, _germ2[i].id, -value * 2);
			match(idx, _germ2[i].id);
		}
		else
		{
			rotate(idx, _germ2[i].id, 180);
			match(idx, _germ2[i].id);
		}
	}
	gpu_process(_buffer);

	for (int i = 0; i < MAX_CORE; i++)
		_bufferSize[i] = 0;

	int a = 0;
	int b = 0;

	for (int i = 0; i < _germ2Size; i++)
	{
		idx = i % MAX_CORE;
		if (type > 0 && type < 3)
		{
			a = _buffer[idx][_bufferSize[idx] + 1];
			b = _buffer[idx][_bufferSize[idx] + 4];
			_bufferSize[idx] += 6;
		}
		else
		{
			a = _germ2[i].similarity;
			b = _buffer[idx][_bufferSize[idx] + 1];
			_bufferSize[idx] += 3;
		}
		if (type == 1)
		{
			if (a == 0)
				answer++;
			else if (b == 0)
				answer++;
		}
		else if (type == 2)
		{
			if (a == ABS(_targetSize - _germ2[i].size))
				answer++;

			else if (b == ABS(_targetSize - _germ2[i].size))
				answer++;
		}
		else
		{
			value = (180 + (b - a)) / 2;
			if (b - value - ABS(_targetSize - _germ2[i].size) == 0)
				answer++;
		}
	}
}

void process()
{
	init();
	GetSize();
	checkItem();
	answer_count(3);
	set_answer(answer);
}
```

### []
