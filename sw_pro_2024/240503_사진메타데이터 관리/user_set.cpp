#include <cstring>
#include <vector>
using namespace std;

#define MAX_PIC (10'000 + 100 * 1'000 + 1)

struct Picture {
	char ID[10];
	char TIME[30];
	char LOC[11];
	char PEOPLE[55];
	int id;
	int date;
	int time;
	vector<const char*> peopleList;

	void update_id() {
		id = atoi(ID);
	}
	void update_time() {
		char *start, *end;
		char year[5], month[3], day[3], hour[3], min[3], sec[3];
		strncpy_s(year, TIME, 4);

		start = strchr(TIME, '/');
		end = strchr(start + 1, '/');
		strncpy(month, start + 1, end - start - 1);

		start = strchr(end, '/');
		end = strchr(start + 1, ',');
		strncpy(day, start + 1, end - start - 1);

		start = end;
		end = strchr(start + 1, ':');
		strncpy(hour, start + 1, end - start - 1);

		start = end;
		end = strchr(start + 1, ':');
		strncpy(min, start + 1, end - start - 1);

		start = end;
		strncpy(sec, start + 1, strlen(start));

		date = atoi(year) * 10'000 + atoi(month) * 100 + atoi(day);
		time = atoi(hour) * 10'000 + atoi(min) * 100 + atoi(sec);
	}
	void update_people() {
		char* ptr = strtok(PEOPLE, ",");
		while (ptr != NULL) {
			peopleList.push_back(ptr);
			ptr = strtok(NULL, ",");
		}
	}
};

Picture pictures[MAX_PIC];
int cnt;

char* copy_metaData(char* metaData, const char* field, char* dest) {
	char* start, * end;

	start = strstr(metaData, field);
	start = strchr(start, '[');
	end = strchr(start, ']');
	strncpy(dest, start + 1, end - start - 1);
	return end + 1;
}

// ==================================================================	
void savePictures(int M, char mPictureList[][200]);

void init(int N, char mPictureList[][200])
{
	cnt = 0;
	for (int i = 0; i < MAX_PIC; i++)
		pictures[i] = {};

	savePictures(N, mPictureList);
}

void savePictures(int M, char mPictureList[][200])
{
	for (int i = 0; i < M; i++) {
		char* start;
		Picture& pic = pictures[cnt + i];

		start = copy_metaData(mPictureList[i], "ID", pic.ID);
		start = copy_metaData(start, "TIME", pic.TIME);
		start = copy_metaData(start, "LOC", pic.LOC);
		start = copy_metaData(start, "PEOPLE", pic.PEOPLE);
		pic.update_id();
		pic.update_time();
		pic.update_people();
	}
	cnt += M;
}

int filterPictures(char mFilter[], int K)
{

	return -1;
}

int deleteOldest(void)
{

	return -1;
}