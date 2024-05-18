#if 1   // 613 ms
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

#define MAX_PICTURES (10'000 + 100 * 1'000 + 1)

struct Picture {
    int id;
    int time;
    char* loc;
    vector<const char*> peopleList;

    bool operator<(const Picture& pic) const { return time < pic.time; }
};
Picture pictures[MAX_PICTURES];
int picIdx;
int oldIdx;

unordered_map<string, vector<int>> locMap;
unordered_map<string, vector<int>> peopleMap;

/////////////////////////////////////////////////////////////////////
void savePictures(int M, char mPictureList[][200]);

// N [1, 10'000]
void init(int N, char mPictureList[][200])
{
    // for (int i = 0; i < MAX_PICTURES; i++) pictures[i] = {};
    for (int i = 0; i < picIdx; i++) pictures[i].peopleList.clear();

    // init
    picIdx = 0;
    oldIdx = 0;
    locMap.clear();
    peopleMap.clear();

    // parsing + sorting + mapping
    savePictures(N, mPictureList);
}

// "ID:[384748],TIME:[2015/2/2,0:21:50],LOC:[school],PEOPLE:[I,dad,mom]"
// M [1, 100]
// 1'000
void savePictures(int M, char mPictureList[][200])
{
    // parsing
    char delim[] = "[],:/";
    for (int i = 0; i < M; i++) {
        auto& pic = pictures[picIdx + i];

        // ID
        char* ptr = strtok(mPictureList[i], delim);
        ptr = strtok(NULL, delim);  pic.id = atoi(ptr);

        // TIME
        ptr = strtok(NULL, delim);
        int YYYY, MM, DD, hh, mm, ss;
        ptr = strtok(NULL, delim);  YYYY = atoi(ptr);
        ptr = strtok(NULL, delim);  MM = atoi(ptr);
        ptr = strtok(NULL, delim);  DD = atoi(ptr);
        ptr = strtok(NULL, delim);  hh = atoi(ptr);
        ptr = strtok(NULL, delim);  mm = atoi(ptr);
        ptr = strtok(NULL, delim);  ss = atoi(ptr);
        pic.time = ((((YYYY * 12 + MM) * 31 + DD) * 24 + hh) * 60 + mm) * 60 + ss;

        // LOC
        ptr = strtok(NULL, delim);
        ptr = strtok(NULL, delim);  pic.loc = ptr;

        // PEOPLE
        ptr = strtok(NULL, delim);
        ptr = strtok(NULL, delim);
        while (ptr != NULL) {
            pic.peopleList.push_back(ptr);
            ptr = strtok(NULL, delim);
        }
    }
    // sorting
    int start = picIdx, end = picIdx + M;
    sort(pictures + start, pictures + end);
    picIdx += M;

    // mapping
    for (int i = start; i < end; i++) {
        auto& pic = pictures[i];
        locMap[pic.loc].push_back(i);
        for (const auto& people: pic.peopleList)
            peopleMap[people].push_back(i);
    }
}

// 100'000
int filterPictures(char mFilter[], int K)
{
    char* ptr = strtok(mFilter, "[]:");
    char* filter = strtok(NULL, "[]:");
    const auto& map = (ptr[0] == 'L')? locMap[filter]: peopleMap[filter];
    int kth = map[map.size() - K];
	return pictures[kth].id;
}

// 1'000
int deleteOldest(void)
{
	return pictures[oldIdx++].id;
}
#endif