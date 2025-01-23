# 230114_XY_Polynomial

## [개요]

- X와 Y의 변수로 이루어진 다항식
- 다항식은 0 ~ 9의 숫자와 X, Y 변수, +, -, ^ 연산자로 이루어져 있다.

## [User API]

### void init()

### void assign(char mName[], char mPolynomial[])

- 이름이 mName이고 mPolynomial로 주어진 다항식을 저장한다.


### void compute(char mNameR[], char mNameA[], char mNameB[], int mOp)

- 이름이 mNameA인 다항식과 이름이 mNameB인 다항식을 mOp에 따라 연산하고 그 결과를 이름이 mNameR인 다항식으로 저장한다.
- mOp=0인 경우 다항식 mNameA와 다항식 mNameB를 더한다.
- mOp=1인 경우 다항식 mNameA에서 다항식 mNameB를 뺀다.
- mOp=2인 경우 다항식 mNameA와 다항식 mNameB를 곱한다.
- mNameA와 mNameB가 같을 수도 있다
- (중요) 계산한 결과의 다항식에서 각 항의 X와 Y의 지수가 모두 1,000 이하이고 항의 개수가 500 이하임을 보장한다.

### int getCoefficient(char mName[], int mDegreeX, int mDegreeY)

- 이름이 mName인 다항식에서 X의 지수가 mDegreeX이고 Y의 지수가 mDegreeY인 항의 계수 값을 찾는다.
- 찾은 계수의 값을 10,000으로 나눈 나머지를 반환한다. 만약 나머지가 음수인 경우 그 나머지 값에 10,000을 더한 값을 반환한다.
- 해당하는 항이 없다면 0을 반환한다.

### int calcPolynomial(char mName[], int mX, int mY)

- 이름이 mNameA인 다항식에 X 변수에 mX 값을 대입하고 Y 변수에 mY 값을 대입하여 값을 계산한다.
- 계산된 값을 10,000으로 나눈 나머지를 반환한다. 만약 나머지가 음수인 경우 그 나머지 값에 10,000을 더한 값을 반환한다.

## [제약사항]

- assign() 함수의 호출 횟수는 1,000 이하이다.
- compute() 함수의 호출 횟수는 9,000 이하이고 compute() 함수 호출 중 mOp = 2인 경우는 1,000 이하이다.
- 모든 함수의 호출 횟수는 50,000 이하이다.
- 모든 다항식의 항의 개수의 총합은 200,000 이하이다.


## [설계]

```cpp
struct Polynomial {};
```