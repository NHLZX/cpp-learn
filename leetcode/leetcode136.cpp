#include <iostream>
#include <vector>
using namespace std;
// 寻找出现两次的数，其他数全是三次
int findtwonums2(vector<int> &nums)
{
    int ans = 0;
    for (int i = 0; i < 32; i++)
    {
        int count = 0;
        for (auto &num : nums)
        {
            if ((num >> i) & 1)
            {
                count++;
            }
            if (count % 3 == 2)
            {
                ans |= 1 << i;
            }
        }
        return ans;
    }
}

int findtwonums(vector<int> &nums)
{
    int ans = 0;
    for (int i = 0; i < 32; i++)
    {
        int count = 0;
        for (auto num : nums)
        {

            if ((num >> i) & 1)
            {
                count++;
            }
        }
        if (count % 3 == 2)
        {
            ans |= 1 << i;
        }
    }
    return ans;
}

int main()
{
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i];
    }
    cout << findtwonums(nums) << endl;
    return 0;
}