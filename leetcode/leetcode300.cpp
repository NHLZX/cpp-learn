#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

int lengthOfLIS(vector<int> &nums)
{
    vector<int> dp(nums.size());
    int num_max = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        dp[i] = 1;
        for (int j = 0; j < i; j++)
        {
            if (nums[i] > nums[j])
            {
                dp[i] = max(dp[i], dp[j] + 1);
                num_max = max(dp[i], num_max);
            }
        }
    }
    return num_max;
}
int main()
{
    string str;

    while (getline(cin, str))
    {
        vector<int> nums;
        stringstream ss(str);
        int num;
        while (ss >> num)
        {
            nums.push_back(num);
        }
        cout << lengthOfLIS(nums) << endl;
    }
}