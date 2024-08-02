#if 1
#include <vector>
#include <set>
#include <cstring>
#include <string>
#include <unordered_map>
using namespace std;

#define MAX_OBJECTS 100'001
#define MAX_LEN     11

enum State { ADDED, REMOVED };

struct Object {
    vector<int> childList;
    set<string> tagList;
    State state;
} objects[MAX_OBJECTS];
int objectCnt;

vector<int> selectedObjList;

bool findTag(const vector<string> tagList, const char mTag[]) {
    for (const auto& tag: tagList)
        if (tag == string(mTag)) return true;
    return false;
}



/////////////////////////////////////////////////////////////////////
// int selectAll(char mTag[]);
// int addChild(char mTag[]);

void init()
{
    for (int i = 0; i < objectCnt; i++) objects[i] = {};
    selectedObjList.clear();
    objectCnt = 0;

    int obj = objectCnt++;
    objects[obj].tagList.insert("root");
    objects[obj].state = ADDED;
    selectedObjList.push_back(obj);
}

int selectAll(char mTag[])
{
    selectedObjList.clear();
    int res = 0;
    for (int obj = 0; obj < objectCnt; obj++) {
        if (objects[obj].state == REMOVED) continue;
        if (objects[obj].tagList.count(mTag)) {
            // selectedObjList.push_back(obj);
            res++;
        }
    }
    return res;
}

int addChild(char mTag[])
{
    int res = 0;
    for (int obj: selectedObjList) {
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
    return 0;
}

int addTag(char mTag[])
{

    return 0;
}
#endif