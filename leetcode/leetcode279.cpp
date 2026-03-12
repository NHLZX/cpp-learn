#include <iostream>
#include <vector>
#include <climits>
using namespace std;
int numsqrt(int num)
{
    vector<int> f(num + 1);
    for (int i = 1; i <= num; i++)
    {
        int num_min = INT_MAX;
        for (int j = 1; j * j <= num; j++)
        {
            num_min = min(num_min, f[i - j * j]);
        }
        f[i] = num_min + 1;
    }
    return f[num];
}
int main()
{
    int num;
    while (cin >> num)
    {
        cout << numsqrt(num) << endl;
    }
    return 0;
}