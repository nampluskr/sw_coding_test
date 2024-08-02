#if 0
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
int objCnt;

string selectedTag;
vector<int> selectedObjList;


/////////////////////////////////////////////////////////////////////
void init()
{
    for (int i = 0; i < objCnt; i++) {
        objects[i].childList.clear();
        objects[i].tagList.clear();
        objects[i].state = ADDED;
    }
    selectedObjList.clear();
    objCnt = 0;

    int obj = objCnt++;
    objects[obj].tagList.insert("root");
    selectedTag = "root";
    selectedObjList.push_back(obj);
}

int selectAll(char mTag[])
{
    selectedObjList.clear();
    int res = 0;
    for (int obj = 0; obj < objCnt; obj++) {
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
    for (const auto& obj: selectedObjList) {
        if (objects[obj].state == REMOVED) continue;
        int child = objCnt++;
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