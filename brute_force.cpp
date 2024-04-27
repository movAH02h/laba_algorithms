#include <bits/stdc++.h>

//----------------------------------------- Laba brute force method -------------------------------------

bool check_in_rect(int x1, int y1, int x2, int y2, int x, int y)
{
    return (x >= x1 && x < x2 && y < y2 && y >= y1) == 1 ? true : false;
}

int modexp(long long int x, long long int y, long long int N)
{
    if (y == 0) return 1;
    long long int z = modexp(x % N, y / 2, N) % N;
    if (y % 2 == 0)
        return (z * z) % N;
    else
        return ((x % N) * ((z * z) % N)) % N;
}

int binpow (int a, int n) {
	int res = 1;
	while (n)
		if (n & 1) {
			res *= a;
			--n;
		}
		else {
			a *= a;
			n >>= 1;
		}
	return res;
}

int main()
{
    std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> rectangles;
    int number_points = 10000;

    for (int i = 0; i < 11; ++i)
    {
	    auto start = std::chrono::steady_clock::now();
        int number_rect = binpow(2, i);

        for (int j = 0; j < number_rect; ++j)
        {
            rectangles.push_back({{10 * j, 10 * j}, {10 * (2 * number_rect - j), 10 * (2 * number_rect - j)}});
        }

        for (int p = 0; p < number_points; ++p)
        {
            int answer = 0;

            int dot_x = modexp(1223 * p, 31, 20 * number_points);
            int dot_y = modexp(2111 * p, 31, 20 * number_points);


            for (int r = 0; r < rectangles.size(); ++r)
            {
                bool check_result = check_in_rect(rectangles[r].first.first,
                                                  rectangles[r].first.second,
                                                  rectangles[r].second.first,
                                                  rectangles[r].second.second,
                                                  dot_x,
                                                  dot_y);
                if (check_result)
                {
                    answer++;
                }
            }
        }
        auto _end = std::chrono::steady_clock::now();

        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(_end - start).count() << "\n";
    }

    return 0;
}