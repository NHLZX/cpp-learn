#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;
// 给你一个字符串数组，请你将 组合在一起。可以按任意顺序返回结果列表。
vector<vector<string>> groupAnagrams(vector<string> &str)
{
    unordered_map<string, vector<string>> map;
    vector<vector<string>> ans;
    for (auto s : str)
    {
        string s1 = s;
        sort(s.begin(), s.end());
        map[s].emplace_back(s1);
    }
    // vector<string> strs;
    for (const auto &[_, strv] : map)
    {
        ans.emplace_back(strv);
    }
    return ans;
}
int main()
{
    int n;
    vector<string> str;
    while (cin >> n)
    {
        string s;
        for (int i = 0; i < n; i++)
        {
            cin >> s;
            str.emplace_back(s);
        }
        vector<vector<string>> ans;
        ans = groupAnagrams(str);
        for (int i = 0; i < ans.size(); ++i)
        {
            cout << "  [";
            for (int j = 0; j < ans[i].size(); ++j)
            {
                cout << "\"" << ans[i][j] << "\"";
                if (j != ans[i].size() - 1)
                    cout << ", ";
            }
            cout << "]\n";
        }
    }
    return 0;
}
