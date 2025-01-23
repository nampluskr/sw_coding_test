#if 0
#include<vector>
#include<set>
using namespace std;

#define MAX_OBJECTS 100'000
using ll = long long;

struct Object {
    vector<int> childList;  // object index list
    set<ll> tagList;        // tag ID list

    bool isRemoved() { return tagList.empty(); } // 폴더가 지워질때 tagList.clear() 수행해서 지워졌음을 표시
    void addSubObject(int obj) { childList.push_back(obj); }
    void removeSubObject(int obj) {
        childList[obj] = childList.back();
        childList.pop_back();
    }   // 맨뒤 원소를 지우는 위치로 이동
    bool existTag(ll tagID) { return tagList.count(tagID); }          // 존재 1 , 없음 0
    bool addTag(ll tagID) { return tagList.insert(tagID).second; }    // 이름 추가하고 추가 됐는지 반환
} objects[MAX_OBJECTS];                                        // insert 반환값 : pair(first: iterator, second: 성공 여부)
int objectCnt;

vector<int> selectedObjList;

ll getTagID(const char str[]) {
    ll tagID = 0;
    for (int i = 0; str[i]; i++) tagID = tagID * 27 + str[i] - 'a';
    return tagID;
}

// dfs
int removeAll(int obj) {
    int cnt = 1;
    for (auto& y : objects[obj].childList) 
        cnt += removeAll(y);
    objects[obj].tagList.clear();   // 지워졌음을 표시
    return cnt;
}

/////////////////////////////////////////////////////////////////////
void init() {

    for (int i = 0; i < objectCnt; i++) objects[i] = {};

    objectCnt = 0;
    int objIdx = objectCnt++;
    objects[objIdx].addTag(getTagID("root"));

    selectedObjList.clear();
    selectedObjList.push_back(objIdx);
}


int selectAll(char mTag[])
{
    ll tagID = getTagID(mTag);
    selectedObjList.clear();
    int res = 0;
    for (int obj = 0; obj < objectCnt; obj++) {
        if (objects[obj].existTag(tagID)) {
            selectedObjList.push_back(obj);
            res++;
        }
    }
    return res;
}


int addChild(char mTag[])
{
    ll tagID = getTagID(mTag);
    int res = 0;
    for (auto& x : selectedObjList) {
        if (objects[x].isRemoved()) continue;
        int objIdx = objectCnt++;
        objects[objIdx].addTag(tagID);
        objects[x].childList.push_back(objIdx);
        res++;
    }
    return res;
}

int removeChild(char mTag[])
{
    ll tagID = getTagID(mTag);
    int res = 0;
    for (auto& x : selectedObjList) {
        if (objects[x].isRemoved()) continue;
        for (int i = objects[x].childList.size() - 1; i >= 0; i--) { // sub folder를 지울때 마지막 원소를 이동시키므로 뒤에서부터 loop를 돈다.
            int y = objects[x].childList[i];                         // or 앞에서부터 돌고 지워지지 않은 경우에만 i를 증가

            if (objects[y].existTag(tagID)) {
                res += removeAll(y);
                objects[x].removeSubObject(i);                    // 마지막 원소를 i로 move
            }
        }
    }
    return res;
}

int addTag(char mTag[])
{
    ll tagID = getTagID(mTag);
    int res = 0;
    for (auto& obj : selectedObjList) {
        if (!objects[obj].isRemoved() && objects[obj].tagList.size() < 5)
            res += objects[obj].addTag(tagID); // set 특성상 어차피 이미 존재하는 이름이면 무시된다.
    }
    return res;
}
#endif