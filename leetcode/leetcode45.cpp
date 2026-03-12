#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
int jump(vector<int> &nums)
{
    int jump_num = 0;
    int end = 0;
    int max_num = 0;
    for (int i = 0; i < nums.size() - 1; i++)
    {
        if (max_num >= i)
        {
            max_num = max(max_num, i + nums[i]);

            if (i == end)
            {
                jump_num++;
                end = max_num;
            }
        }
    }
    return jump_num;
}
int main()
{
    string nums_s;
    vector<int> nums;
    while (getline(cin, nums_s))
    {
        stringstream ss(nums_s);
        int num;
        while (ss >> num)
        {
            nums.push_back(num);
        }
        cout << jump(nums) << endl;
    }
    return 0;
}