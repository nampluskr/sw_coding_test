#if 0
#include <vector>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

#define MAX_OBJECTS 100'000

enum State { ADDED, REMOVED };

struct Object {
    vector<int> childList;  // object index list
    set<string> tagList;    // tag ID list
    State state;
} objects[MAX_OBJECTS];
int objectCnt;

vector<int> selectedObjList;

// dfs
int removeAll(int obj) {
    int cnt = 1;
    for (int child : objects[obj].childList) {
        if (objects[child].state == REMOVED) continue;
        cnt += removeAll(child);
        objects[child].state = REMOVED;    
    }
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
        objects[obj].childList.push_back(child);
        objects[child].tagList.insert(mTag);
        res++;
    }
    return res;
}

int removeChild(char mTag[])
{
    int res = 0;
    for (int obj : selectedObjList) {
        if (objects[obj].state == REMOVED) continue;

        for (int i = objects[obj].childList.size() - 1; i >= 0; i--) {
        // for (sint i = 0; i < objects[obj].childList.size(); i++) {
            int child = objects[obj].childList[i];
            if (objects[child].state == REMOVED) continue;

            if (objects[child].tagList.count(mTag)) {
                res += removeAll(child);

                auto& vec = objects[obj].childList;
                vec.erase(find(vec.begin(), vec.end(), child));
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
        if (objects[obj].tagList.count(mTag)) continue;
        if (objects[obj].tagList.size() >= 5) continue;

        objects[obj].tagList.insert(mTag);
        res++;
    }
    return res;
}
#endif