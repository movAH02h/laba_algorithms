#include <bits/stdc++.h>

//------------------------------------ Laba map method --------------------------------------------

int bin_search(std::vector<int>& arr, int low, int high, int target)
{
    if (high >= low) {

        int mid = low + (high - low) / 2;

        if (arr[mid] == target)
            return mid;

        if (arr[mid] > target)
            return bin_search(arr, low, mid - 1, target);

        return  bin_search(arr, mid + 1, high, target);
    }
    return high;
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
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> rectangles;
    std::set<int> x_set;
    std::set<int> y_set;
    int number_points = 10000;

    for (int i = 0; i < 11; ++i)
    {
	    auto start = std::chrono::steady_clock::now();
        int number_rect = binpow(2, i);

        for (int j = 0; j < number_rect; ++j)
        {
            int x1 = 10 * j;
            int y1 = 10 * j;
            int x2 = 10 * (2 * number_rect - j);
            int y2 = 10 * (2 * number_rect - j);
            x_set.insert(x1);
            x_set.insert(x2);
            y_set.insert(y1);
            y_set.insert(y2);
            rectangles.push_back({{x1, y1}, {x2, y2}});
        }
        x_set.insert(0);
        y_set.insert(0);

        std::vector<int> x_vector(x_set.begin(), x_set.end());
        std::vector<int> y_vector(y_set.begin(), y_set.end());

        std::vector<std::vector<int>> matrix(y_vector.size(), std::vector<int>(x_vector.size(), 0));
        

        for (int r = 0; r < rectangles.size(); ++r)
        {
            int x1 = rectangles[r].first.first;
            int y1 = rectangles[r].first.second;
            int x2 = rectangles[r].second.first;
            int y2 = rectangles[r].second.second;

            //auto [x1, y1] = rectangles[r].first;
            //auto [x2, y2] = rectangles[r].second;

            int x_left = bin_search(x_vector, 0, x_vector.size() - 1, x1),
            x_right = bin_search(x_vector, 0, x_vector.size() - 1, x2),
            y_left = bin_search(y_vector, 0, y_vector.size() - 1, y1),
            y_right = bin_search(y_vector, 0, y_vector.size() - 1, y2);

            for (int i1 = y_left; i1 < y_right; ++i1)
            {
                for (int j1 = x_left;j1 < x_right; ++j1)
                {
                    matrix[i1][j1]++;
                }
            }
        }

        std::vector<int> answer;

        for (int p = 0; p < number_points; ++p)
        {
            int x = modexp(1223 * p, 31, 20 * number_points);
            int y = modexp(2111 * p, 31, 20 * number_points);

            int index_row = bin_search(y_vector, 0, y_vector.size() - 1, y);
            int index_column = bin_search(x_vector, 0, x_vector.size() - 1, x);

            answer.push_back(matrix[index_row][index_column]);
        }
        auto _end = std::chrono::steady_clock::now();

        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(_end - start).count() << "\n";
    }





    return 0;
}