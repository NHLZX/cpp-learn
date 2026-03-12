#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;
// 盛水最多的容器
int maxArea(vector<int> &height)
{
    int maxarea = 0;
    int left = 0;
    int right = height.size() - 1;
    while (left < right)
    {
        if (height[left] < height[right])
        {
            maxarea = max(maxarea, height[left] * (right - left));
            left++;
        }
        else
        {
            maxarea = max(maxarea, height[right] * (right - left));
            right--;
        }
    }
    return maxarea;
}
int main()
{
    string str;
    vector<int> nums;
    while (getline(cin, str))
    {
        stringstream ss(str);
        int num;
        while (ss >> num)
        {
            nums.push_back(num);
        }
        cout << maxArea(nums) << endl;
    }
}