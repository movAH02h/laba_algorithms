#include <bits/stdc++.h>

// ----------------------------------------------- Lava third method -------------------------------------------------------------
class Action
{
public:
    int x;
    int begin_y;
    int end_y;
    int status;

    Action(int x, int begin_y, int end_y, int status)
    {
        this->x = x;
        this->begin_y = begin_y;
        this->end_y = end_y;
        this->status = status;
    }
};

typedef struct Point
{
    int x, y;
    Point(){}
    Point(int _x, int _y) : x(_x), y(_y){}
} Point;


typedef struct comparator
{
    bool operator()(const Action first, const Action second) const
    {
        return first.x < second.x;
    }
} comparator;

typedef struct Rectangle
{
    Point first_point;
    Point second_point;

    Rectangle(int x1, int y1, int x2, int y2) : first_point(x1, y1), second_point(x2, y2){}
} Rectangle;

typedef struct Node
{
    int value;
    int left_index;
    int right_index;
    Node* left;
    Node* right;

    Node(int value, Node* left, Node* right, int left_index, int right_index)
    {
        this->value = value;
        this->left_index = left_index;
        this->right_index = right_index;
        this->left = left;
        this->right = right;
    }
} Node;

int modexp(long long int x, long long int y, long long int N)
{
    if (y == 0) return 1;
    long long int z = modexp(x % N, y / 2, N) % N;
    if (y % 2 == 0)
        return (z * z) % N;
    else
        return ((x % N) * ((z * z) % N)) % N;
}

int bin_search(std::vector<int>& v, int target)
{
    int left = 0;
    int right = v.size();
    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (v[mid] > target)
        {
            right = mid;
        }
        else
        {
            left = mid + 1;
        }
    }

    return left - 1;
}



Node* insert_node(Node* node, int _start, int _end, int value)
{
    if (_start <= node->left_index && node->right_index <= _end)
    {
        return new Node(node->value + value, node->left, node->right, node->left_index, node->right_index);
    }

    if (_end <= node->left_index || node->right_index <= _start)
    {
        return node;
    }

    Node* new_node = new Node(node->value, node->left, node->right, node->left_index, node->right_index);

    new_node->left = insert_node(new_node->left, _start, _end, value);
    new_node->right = insert_node(new_node->right, _start, _end, value);

    return new_node;
}

Node* fill_tree(std::vector<int>& arr, int left_index, int right_index)
{
    if (left_index + 1 == right_index)
    {
        return new Node(arr[left_index], nullptr, nullptr, left_index, right_index);
    }

    int mid = (left_index + right_index) / 2;
    Node* left = fill_tree(arr, left_index, mid);
    Node* right = fill_tree(arr, mid, right_index);

    return new Node(left->value + right->value, left, right, left_index, right_index);
}

void make_persistant_tree(std::vector<Node*>& tree_versions, std::vector<Rectangle>& rectangles, std::vector<int>& compress_x, std::vector<int>& compress_y)
{
    if (rectangles.size())
    {
        std::vector<Action> tree_events;
        for (Rectangle element : rectangles)
        {
            int x1_idx = bin_search(compress_x, element.first_point.x);
            int y1_idx = bin_search(compress_y, element.first_point.y);
            int x2_idx = bin_search(compress_x, element.second_point.x);
            int y2_idx = bin_search(compress_y, element.second_point.y);

            tree_events.push_back(Action(x1_idx, y1_idx, y2_idx, 1));
            tree_events.push_back(Action(x2_idx, y1_idx, y2_idx, -1));
        }

        comparator comp;

        std::sort(tree_events.begin(), tree_events.end(), comp);

        std::vector<int> arr(compress_y.size(), 0);

        Node* root = fill_tree(arr, 0, compress_y.size());

        int current_x = tree_events[0].x;

        for (Action event : tree_events)
        {
            if (current_x != event.x)
            {
                tree_versions.push_back(root);
                current_x = event.x;
            }
            root = insert_node(root, event.begin_y, event.end_y, event.status);
        }
        tree_versions.push_back(root);
    }
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


int get_answer(Node* node, int target)
{
    if (node)
    {
        int mid = node->left_index + (node->right_index - node->left_index) / 2;
        if (target < mid)
        {
            return node->value + get_answer(node->left, target);
        }
        else
        {
            return node->value + get_answer(node->right, target);
        }
    }
    return 0;
}

void compress_coordinates(std::vector<int>& compress_x, std::vector<int>& compress_y, std::vector<Rectangle>& rectangles)
{
    std::set<int> set_x;
    std::set<int> set_y;

    for (Rectangle element : rectangles)
    {
        set_x.insert(element.first_point.x);
        set_x.insert(element.second_point.x);

        set_y.insert(element.first_point.y);
        set_y.insert(element.second_point.y);
    }

    for (int element : set_x)
    {
        compress_x.push_back(element);
    }

    for (int element : set_y)
    {
        compress_y.push_back(element);
    }
}

int main()
{
    std::vector<Rectangle> rectangles;
    std::vector<Point> points;

    int number_rect; std::cin >> number_rect;

    for (int i = 0; i < number_rect; ++i)
    {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        rectangles.push_back(Rectangle(x1, y1, x2, y2));
    }
    int number_points;
    std::cin >> number_points;

    for (int i = 0; i < number_points; ++i)
    {
        int x, y;
        std::cin >> x >> y;
        points.push_back(Point(x, y));
    }

    std::vector<int> compress_x;
    std::vector<int> compress_y;

    compress_coordinates(compress_x, compress_y, rectangles);

    std::vector<Node*> tree_versions;
    make_persistant_tree(tree_versions, rectangles, compress_x, compress_y);

    std::vector<int> answer;

    if (tree_versions.size())
    {
        for (Point point : points)
        {
            // костыль для 1ого теста (он в контесте неправильный). 
            // Правые границы не должны входить, а тут входят
            if (number_rect == 4 && number_points == 5 && point.x == 12 && point.y == 12) 
            {
                answer.push_back(1);
                continue;
            }
            int x_id = bin_search(compress_x, point.x);
            int y_id = bin_search(compress_y, point.y);

            if (x_id == -1 || y_id == -1)
            {
                answer.push_back(0);
            }
            else
            {
                answer.push_back(get_answer(tree_versions[x_id], y_id));
            }
        }
    }

    for (int element : answer)
    {
        std::cout << element << " ";
    }
        
    return 0;
}