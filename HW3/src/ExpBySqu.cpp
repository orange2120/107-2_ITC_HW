#include <iostream>
using namespace std;

int main()
{
	uint64_t b, e, n;
	uint64_t r = 1;
	cin >> b >> e >> n;
	while (e > 0)
	{
		if (e & 1)
				r = (r * b) % n;
		e >>= 1;
		b = (b * b) % n;
	}

	cout << r << endl;
}
