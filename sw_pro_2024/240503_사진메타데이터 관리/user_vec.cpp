#if 0
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include <queue>
#include <set>
#include <cstring>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

#define MAX_PIC (10'000 + 100*1'000 + 1)
#define DELETED  1

struct Picture {
	int ID;
	int date;
	int time;
	char loc[11];
	char people[55];
	vector<const char*> peopleList;
    int state;
};

Picture pictures[MAX_PIC];
int cnt;

char ID[10], DATETIME[21], LOC[11], PEOPLE[55];

void get_metaData(char* mPicture) {
	char* start, * end;

	// ID 임시 저장
	start = strchr(mPicture, '[');
	end = strchr(start + 1, ']');
	strncpy_s(ID, start + 1, end - start - 1);

	// TIME 임시 저장
	start = strchr(end + 1, '[');
	end = strchr(start + 1, ']');
	strncpy_s(DATETIME, start + 1, end - start - 1);

	// LOC 임시 저장
	start = strchr(end + 1, '[');
	end = strchr(start + 1, ']');
	strncpy_s(LOC, start + 1, end - start - 1);

	// PEOPLE 임시 저장
	start = strchr(end + 1, '[');
	end = strchr(start + 1, ']');
	strncpy_s(PEOPLE, start + 1, end - start - 1);
}

void set_metaData(Picture& pic) {
	char* start, * end;

	// str DATE -> int
	char year[5], month[3], day[3], hour[3], min[3], sec[3];
	strncpy_s(year, DATETIME, 4);

	start = strchr(DATETIME, '/');
	end = strchr(start + 1, '/');
	strncpy_s(month, start + 1, end - start - 1);

	start = strchr(end, '/');
	end = strchr(start + 1, ',');
	strncpy_s(day, start + 1, end - start - 1);

	// str TIME -> int
	start = end;
	end = strchr(start + 1, ':');
	strncpy_s(hour, start + 1, end - start - 1);

	start = end;
	end = strchr(start + 1, ':');
	strncpy_s(min, start + 1, end - start - 1);

	start = end;
	strncpy_s(sec, start + 1, strlen(start));

	pic.ID = atoi(ID);
	pic.date = atoi(year) * 10'000 + atoi(month) * 100 + atoi(day);
	pic.time = atoi(hour) * 10'000 + atoi(min) * 100 + atoi(sec);
	strcpy_s(pic.loc, LOC);
	strcpy_s(pic.people, PEOPLE);

	char* ptr = strtok(pic.people, ",");
	while (ptr != NULL) {
		pic.peopleList.push_back(ptr);
		ptr = strtok(NULL, ",");
	}
}

struct Data {
	int date, time, picIdx;

	bool operator<(const Data& data) const {
		return (date > data.date) || (date == data.date && time > data.time);
	}
};

unordered_map<int, int> picMap;
unordered_map<string, vector<Data>> locMap;
unordered_map<string, vector<Data>> peopleMap;
priority_queue<Data> oldestPQ;


// ==================================================================
void savePictures(int M, char mPictureList[][200]);

void init(int N, char mPictureList[][200])
{
	cnt = 0;
	picMap.clear();
    locMap.clear();
	peopleMap.clear();
	while (!oldestPQ.empty()) oldestPQ.pop();

	for (int i = 0; i < MAX_PIC; i++)
		pictures[i] = {};

	savePictures(N, mPictureList);
}

void savePictures(int M, char mPictureList[][200])
{
	for (int i = 0; i < M; i++) {
		get_metaData(mPictureList[i]);
		Picture& pic = pictures[cnt];
		set_metaData(pic);

		picMap[pic.ID] = cnt;
		locMap[pic.loc].push_back({ pic.date, pic.time, cnt });
		for (const auto& p : pic.peopleList)
			peopleMap[p].push_back({ pic.date, pic.time, cnt });
		oldestPQ.push({ pic.date, pic.time, cnt });
		cnt++;
	}
}

int filterPictures(char mFilter[], int K)
{
	char* start = strchr(mFilter, '[');
	char* end = strchr(start, ']');
	char strFilter[11];
	strncpy_s(strFilter, start + 1, end - start - 1);
    auto& vec = (mFilter[0]  == 'L')? locMap[strFilter]: peopleMap[strFilter];
	int picIdx = 0;

    // quick select
    vector<Data> v;
    for (const auto& data: vec)
        if (pictures[data.picIdx].state != DELETED)
            v.push_back(data);

    nth_element(v.begin(), v.begin() + K - 1, v.end());
    picIdx = v[K - 1].picIdx;

	return pictures[picIdx].ID;
}

int deleteOldest(void)
{
	int picIdx = oldestPQ.top().picIdx; oldestPQ.pop();
	pictures[picIdx].state = DELETED;

	return pictures[picIdx].ID;
}
#endif