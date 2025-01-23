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
    vector<string> tagList;    // tag ID list
    State state;
} objects[MAX_OBJECTS];
int objectCnt;

vector<int> selectedObjList;

bool findTag(const vector<string> tagList, const char mTag[]) {
    for (const auto& tag: tagList)
        if (tag == mTag) return true;
    return false;
}

// dfs
int dfs(int obj, int cnt) {
    if (objects[obj].state != REMOVED) { 
        objects[obj].state = REMOVED;
        for (int child : objects[obj].childList)
            cnt += dfs(child, cnt + 1);
        return cnt;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
void init() {

    for (int i = 0; i < objectCnt; i++) objects[i] = {};
    selectedObjList.clear();
    objectCnt = 0;

    int obj = objectCnt++;
    objects[obj].tagList.push_back("root");
    selectedObjList.push_back(obj);
}

int selectAll(char mTag[])
{
    selectedObjList.clear();
    int res = 0;
    // for (int obj = 0; obj < objectCnt; obj++) {
    //     if (objects[obj].state == REMOVED) continue;
    //     if (findTag(objects[obj].tagList, mTag)) {
    //         selectedObjList.push_back(obj);
    //         res++;
    //     }
    // }
    return res;
}

int addChild(char mTag[])
{
    int res = 0;
    // for (int obj : selectedObjList) {
    //     if (objects[obj].state == REMOVED) continue;
    //     int child = objectCnt++;
    //     objects[obj].childList.push_back(child);
    //     objects[child].tagList.push_back(mTag);
    //     res++;
    // }
    return res;
}

int removeChild(char mTag[])
{
    int res = 0;
    // for (int obj : selectedObjList) {
    //     if (objects[obj].state == REMOVED) continue;
    //     res += dfs(obj, 0);
    // }
    return res;
}

int addTag(char mTag[])
{
    int res = 0;
    // for (int obj : selectedObjList) {
    //     if (objects[obj].state == REMOVED) continue;
    //     if (findTag(objects[obj].tagList, mTag)) continue;
    //     if (objects[obj].tagList.size() >= 5) continue;

    //     objects[obj].tagList.push_back(mTag);
    //     res++;
    // }
    return res;
}
#endif