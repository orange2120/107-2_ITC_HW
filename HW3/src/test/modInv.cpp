#include <iostream>
using namespace std;


int64_t modInverse(uint64_t, uint64_t);

int main()
{
	uint64_t a,b;
	cin >> a >> b;
	cout << modInverse(a,b) << endl;
	return 0;
}

int64_t modInverse(uint64_t a, uint64_t b)
{
	uint64_t b0 = b, t, q;
	int64_t x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}
