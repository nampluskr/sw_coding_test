#if 1
#include <vector>
#include <set>
#include <string>
using namespace std;

#define MAX_OBJECTS 100'000

enum State { ADDED, REMOVED };

struct Object {
    vector<int> childList;  // object index list
    set<string> tagList;    // tag ID list
    State state;
} objects[MAX_OBJECTS];                                        // insert 반환값 : pair(first: iterator, second: 성공 여부)
int objectCnt;

vector<int> selectedObjList;

// dfs
int removeAll(int obj) {
    int cnt = 1;
    for (int child : objects[obj].childList) 
        cnt += removeAll(child);
    objects[obj].tagList.clear();   // 지워졌음을 표시
    objects[obj].state = REMOVED;
    return cnt;
}

/////////////////////////////////////////////////////////////////////
void init() {

    for (int i = 0; i < objectCnt; i++) objects[i] = {};
    selectedObjList.clear();
    objectCnt = 0;

    int objIdx = objectCnt++;
    objects[objIdx].tagList.insert("root");
    selectedObjList.push_back(objIdx);
}

int selectAll(char mTag[])
{
    selectedObjList.clear();
    int res = 0;
    for (int obj = 0; obj < objectCnt; obj++) {
        if (objects[obj].state == REMOVED) continue;
        if (objects[obj].tagList.count(mTag)) {
            selectedObjList.push_back(obj);
            res++;
        }
    }
    return res;
}

int addChild(char mTag[])
{
    int res = 0;
    for (int obj : selectedObjList) {
        if (objects[obj].state == REMOVED) continue;
        int child = objectCnt++;
        objects[child].tagList.insert(mTag);
        objects[obj].childList.push_back(child);
        res++;
    }
    return res;
}

int removeChild(char mTag[])
{
    int res = 0;
    for (int obj : selectedObjList) {
        if (objects[obj].state == REMOVED) continue;
        for (int i = objects[obj].childList.size() - 1; i >= 0; i--) { // sub folder를 지울때 마지막 원소를 이동시키므로 뒤에서부터 loop를 돈다.
            int child = objects[obj].childList[i];                         // or 앞에서부터 돌고 지워지지 않은 경우에만 i를 증가
            if (objects[child].tagList.count(mTag)) {
                res += removeAll(child);
                // 마지막 원소를 i로 move
                objects[obj].childList[i] = objects[obj].childList.back();
                objects[obj].childList.pop_back();
            }
        }
    }
    return res;
}

int addTag(char mTag[])
{
    int res = 0;
    for (int obj : selectedObjList) {
        if (objects[obj].state == REMOVED) continue;
        if (objects[obj].tagList.size() >= 5) continue;
        if (objects[obj].tagList.count(mTag)) continue;

        objects[obj].tagList.insert(mTag);
        res++;
    }
    return res;
}
#endif