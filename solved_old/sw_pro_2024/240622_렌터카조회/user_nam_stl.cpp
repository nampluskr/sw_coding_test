#include <vector>
#include <queue>
using namespace std;

#define MAX_N	100'001

// companies
vector<int> carList[MAX_N];

// cars
struct Car {
	int price, mCarID, number;
	bool operator<(const Car& car) const {
		return (price > car.price) || (price == car.price && mCarID > car.mCarID);
	}
} cars[MAX_N];

struct CarInfo {
	int color, seat, type, size;
} carInfo[MAX_N];

// rent
struct Date { int start, end; };
vector<Date> booking[MAX_N];
priority_queue<Car> carMap[MAX_N];	// carMap[0 ~ 9'999]

int encode(const CarInfo& info) {
	return (info.color - 1) * 1000 + (info.seat - 2) * 100 + info.type * 10 + info.size;
}

bool not_overlap(int mCarID, int start, int end) {
	for (const auto& date : booking[mCarID])
		if (end > date.start && date.end> start) return false;
	return true;
}

////////////////////////////////////////////////////////////////////
void init(int N)
{
	for (int i = 0; i <= N; i++) carList[i].clear();
	for (int i = 0; i < MAX_N; i++) booking[i].clear(), carMap[i] = {};
}

void add(int mCarID, int mCompanyID, int mCarInfo[])
{
	carList[mCompanyID].push_back(mCarID);
	cars[mCarID] = { mCarInfo[5], mCarID, mCarInfo[4]};
	carInfo[mCarID] = { mCarInfo[0], mCarInfo[1], mCarInfo[2], mCarInfo[3] };
	carMap[encode(carInfo[mCarID])].push(cars[mCarID]);
}

int rent(int mCondition[])
{
	int start = mCondition[0], end = mCondition[1];
	CarInfo info = { mCondition[2], mCondition[3], mCondition[4], mCondition[5] };
	auto& pq = carMap[encode(info)];
	vector<int> popped;

	int res = -1;
	while (!pq.empty()) {
		auto car = pq.top(); pq.pop();
		int mCarID = car.mCarID;

		if (car.price != cars[mCarID].price) continue;
		if (car.number == 0) continue;
		if (car.price < 1) continue;

		popped.push_back(mCarID);
		if (not_overlap(mCarID, start, end)) {
			booking[mCarID].push_back({ start, end });
			cars[mCarID].number--;
			res = mCarID;
			break;
		}
	}
	for (int mCarID : popped) pq.push(cars[mCarID]);
	return res;
}

int promote(int mCompanyID, int mDiscount)
{
	int res = 0;
	for (int mCarID : carList[mCompanyID]) {
		if (cars[mCarID].price == 0) continue;
		if (cars[mCarID].number == 0) continue;

		cars[mCarID].price -= mDiscount;
		if (cars[mCarID].price < 0) cars[mCarID].price = 0;

		res += cars[mCarID].price;
		carMap[encode(carInfo[mCarID])].push(cars[mCarID]);
	}
	return res;
}