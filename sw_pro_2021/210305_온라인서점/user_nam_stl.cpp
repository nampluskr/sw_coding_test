#include <vector>
#include <queue>
using namespace std;

#define MAX_BOOKS   100'000
#define MAX_AUTHORS 10'001
#define MAX_CATEGORIES  21

inline int min(int a, int b) { return (a < b)? a: b; };

int N;  // N : 저자의 수 (3 ≤ N ≤ 10,000), 저자의 ID 는 1 이상 N 이하이다.
int C;  // C : 책 카테고리의 수 (3 ≤ C ≤ 20)

// books
enum State { ADDED, REMOVED };

struct Book {
    int cID, price, num;
    State state;
    int stock;      // 재고량
    int sales;      // 판매량
    vector<int> authorList;
} books[MAX_BOOKS];

struct BookData {
    int sales, bID;
    bool operator<(const BookData& book) const {
        return sales < book.sales || (sales == book.sales && bID < book.bID);
    }
};
priority_queue<BookData> bookPQ[MAX_CATEGORIES];
int bookCnt[MAX_CATEGORIES];

// authors
struct Author {
    int income, bID;
} authors[MAX_AUTHORS];

struct AuthorData {
    int income, aID;
    bool operator<(const AuthorData& author) const {
        return income < author.income || (income == author.income && aID > author.aID);
    }
};
priority_queue<AuthorData> authorPQ;

////////////////////////////////////////////////////////////////////////
void init(int N, int C)
{
    ::N = N, ::C = C;
    for (int i = 0; i < MAX_BOOKS; i++) books[i] = {};
    for (int i = 0; i < MAX_AUTHORS; i++) authors[i] = {};
    for (int i = 0; i < MAX_CATEGORIES; i++) {
        bookPQ[i] = {};
        bookCnt[i] = 0;
    }
    authorPQ = {};

    for (int aID = 1; aID <= N; aID++) authorPQ.push({ 0, aID });
}

void addBookInfo(int bID, int cID, int price, int num, int aIDs[])
{
    books[bID] = { cID, price, num, ADDED };
    books[bID].stock++;
    for (int i = 0; i < num; i++) {
        books[bID].authorList.push_back(aIDs[i]);
        // authors[i] = { 0, bID };
        // authorPQ.push({ 0, aIDs[i] });
    }

    bookPQ[cID].push({ 0, bID });   bookCnt[cID]++;
    bookPQ[0].push({ 0, bID });     bookCnt[0]++;
}

void addBookStock(int bID, int cnt)
{
    books[bID].stock += cnt;
}

void removeBookInfo(int bID)
{
    books[bID].state = REMOVED;
    bookCnt[books[bID].cID]--;
    bookCnt[0]--;
}

int sellBooks(int bID, int cnt)
{
    int res = 0;
    auto& book = books[bID];
    if (book.stock >= cnt) {

        book.stock -= cnt;
        book.sales += cnt;
        bookPQ[book.cID].push({ book.sales, bID });
        bookPQ[0].push({ book.sales, bID });

        for (const auto aID: book.authorList) {
            authors[aID].income += book.price * cnt / book.num;
            authorPQ.push({ authors[aID].income, aID });
        }
        res = 1;
    }
    return res;
}

// 만약 판매량이 같을 경우, 책의 ID 값이 더 큰 것이 우선이다.
// bIDs[] 배열에는 절판 되지 않은 책들만 저장되어 있어야 함에 유의하라.
int getBestSeller(int cID, int bIDs[])
{
    auto& pq = bookPQ[cID];
    vector<int> popped;
    int cnt = 0;

    while (!pq.empty() && cnt < 3) {
        auto book = pq.top(); pq.pop();

        if (books[book.bID].state == REMOVED) continue;
        if (book.sales != books[book.bID].sales) continue;

        popped.push_back(book.bID);
        bIDs[cnt++] = book.bID;
    }
    for (int bID: popped) pq.push({ books[bID].sales, bID });

    int res = min(3, bookCnt[cID]);
    return res;
}

// 이때, 저자의 수익은 절판된 책에서 올린 수익도 포함한다.
// 수익이 같다면 저자의 ID가 작은 것을 우선으로 aIDs[] 배열에 저장하여 반환한다.
void getBestAuthors(int aIDs[])
{
    auto& pq = authorPQ;
    vector<int> popped;
    int cnt = 0;

    while (!pq.empty() && cnt < 3) {
        auto author = pq.top(); pq.pop();

        if (author.income != authors[author.aID].income) continue;

        popped.push_back(author.aID);
        aIDs[cnt++] = author.aID;
    }
    for (int aID: popped) pq.push({ authors[aID].income, aID });
}