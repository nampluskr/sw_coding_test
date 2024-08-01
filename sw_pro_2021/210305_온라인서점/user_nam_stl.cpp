#include <vector>
#include <queue>
using namespace std;

#define MAX_BOOKS   100'000
#define MAX_AUTHORS 10'001
#define MAX_CATEGORIES  21

inline int min(int a, int b) { return (a < b)? a: b; };

// books
enum State { ADDED, REMOVED };

struct Book {
    int cID, price, num;
    State state;    // 입고 (ADDED), 절판 (REMOVED)
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
    // for (int i = 0; i < MAX_BOOKS; i++) books[i] = {};
    for (int i = 1; i < N; i++) authors[i] = {};
    for (int i = 0; i < C; i++) {
        bookPQ[i] = {};
        bookCnt[i] = 0;
    }
    authorPQ = {};

    // 저자의 ID 는 1 이상 N 이하이다.
    for (int aID = 1; aID <= N; aID++) authorPQ.push({ 0, aID });
}

void addBookInfo(int bID, int cID, int price, int num, int aIDs[])
{
    books[bID] = { cID, price, num, ADDED };
    books[bID].stock++;
    for (int i = 0; i < num; i++)
        books[bID].authorList.push_back(aIDs[i]);

    bookPQ[cID].push({ 0, bID });
    bookCnt[cID]++;
    
    bookPQ[0].push({ 0, bID });
    bookCnt[0]++;
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
    for (int bID: popped)
        pq.push({ books[bID].sales, bID });

    return min(3, bookCnt[cID]);
}

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
    for (int aID: popped)
        pq.push({ authors[aID].income, aID });
}