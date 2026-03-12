#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
using namespace std;
int longestConsecutive(vector<int> &nums)
{
    unordered_set<int> set;
    for (auto num : nums)
    {
        set.insert(num);
    }
    int longest_nums = 0;
    for (auto num_s : set)
    {
        if (!set.count(num_s - 1))
        {
            int count = 1;
            int cur_num = num_s;
            while (set.count(cur_num + 1))
            {
                cur_num++;
                count++;
            }
            longest_nums = max(longest_nums, count);
        }
    }
    return longest_nums;
}
int main()
{
    string word;

    while (getline(cin, word))
    {
        stringstream ss(word);
        int num;
        vector<int> nums;
        while (ss >> num)
        {
            nums.push_back(num);
        }
        cout << longestConsecutive(nums) << endl;
    }
    return 0;
}