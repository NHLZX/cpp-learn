#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// 核心逻辑函数
int solve(int n, vector<int> &digits)
{
    // 1. 预处理
    string s = to_string(n);
    int len = s.length();
    sort(digits.begin(), digits.end()); // 确保数字集合有序

    // 记录每一位匹配的是 digits 中的第几个下标
    // 比如 n=2533, digits={2,4,9}
    // 第一位匹配了2 (下标0)，path就存 [0]
    vector<int> path;

    int i = 0;
    // 2. 贪心匹配阶段
    // 尝试每一位都跟 n 保持一致
    for (; i < len; ++i)
    {
        int target = s[i] - '0';
        int match_idx = -1;

        // 在 digits 中找有没有等于 target 的
        for (int k = 0; k < digits.size(); ++k)
        {
            if (digits[k] == target)
            {
                match_idx = k;
                break;
            }
        }

        if (match_idx != -1)
        {
            path.push_back(match_idx); // 匹配成功，记录下标
        }
        else
        {
            break; // 匹配失败（没有相等的），跳出循环，准备回溯
        }
    }

    // 3. 回溯处理阶段
    // i 是中断的位置。
    // 如果 i == len，说明完全匹配了（n=222, A={2}），这不符合“小于”，必须回溯最后一位
    // 如果 i < len，说明在 i 位置没找到相等的，需要从 i 开始往前找“更小的”

    // 此时 path 里存的是 [0, i-1] 位匹配的下标
    for (int k = i; k >= 0; --k)
    {
        int target_val;

        if (k == len)
        {
            // 这是一个边界修正。如果完全匹配了，我们从最后一位开始找更小的
            target_val = digits[path.back()] + 1; // 假装目标比当前大一点，这样下面逻辑就能找更小的
            // 实际上直接看 else 分支逻辑更好理解，这里只是为了复用代码
            // 让我们手动修正一下逻辑：
            k--;
        }

        // 我们要找一个数字，它比“当前原本想匹配的数字”小
        // 情况A (k == i): 我们在这一位断开了，我们要找比 s[k] 小的最大数
        // 情况B (k < i):  我们回溯到了之前匹配好的位，我们要找比 path[k] 对应的值更小的数

        int limit_val; // 上界数值
        if (k == i)
        {
            limit_val = s[k] - '0';
        }
        else
        {
            // 取出刚才匹配的那个数
            limit_val = digits[path[k]];
            path.pop_back(); // 这一位既然要换更小的，原本的就扔掉
        }

        // 在 digits 中找比 limit_val 小的最大的数
        // 从后往前遍历 digits
        for (int d_idx = digits.size() - 1; d_idx >= 0; --d_idx)
        {
            if (digits[d_idx] < limit_val)
            {
                // 找到了！
                // 1. 这一位填这个更小的数
                // 2. 后面所有的位填 digits 里的最大数

                int res = 0;
                // 先把 path 里保留的前缀算出来
                for (int idx : path)
                    res = res * 10 + digits[idx];

                // 加上当前这一位
                res = res * 10 + digits[d_idx];

                // 加上后面剩下的位 (总长度 len，现在已经有了 k+1 位)
                // 剩下的位数 = len - 1 - k
                for (int rest = 0; rest < len - 1 - k; ++rest)
                {
                    res = res * 10 + digits.back();
                }
                return res;
            }
        }
        // 如果这一层循环没找到更小的，k 会自减，回溯到上一位
    }

    // 4. 降维打击
    // 如果循环结束还没 return，说明 n 的最高位都比 A 里最小值小
    // 或者完全匹配且无法变小
    // 此时位数减一，全填最大值
    if (len - 1 == 0)
        return -1; // 题目边界，看是否允许

    int res = 0;
    for (int j = 0; j < len - 1; ++j)
    {
        res = res * 10 + digits.back();
    }
    return res;
}

class Soultion
{
public:
    long long maxnum = -1;
    void dfs(vector<int> &nums, bool isless, int index, string &target, long long currentnum)
    {
        if (index >= target.length())
        {
            maxnum = max(currentnum, maxnum);
            return;
        }
        for (int i = nums.size() - 1; i >= 0; i--)
        {
            int d = nums[i];
            if (isless || d < target[index] - '0')
            {
                dfs(nums, true, index + 1, target, currentnum * 10 + d);
            }
            else if (d == target[index] - '0')
            {
                dfs(nums, false, index + 1, target, currentnum * 10 + d);
            }
        }
    }
    int lessn(vector<int> &nums, int n)
    {
        sort(nums.begin(), nums.end()); // 必须排序
        string s = to_string(n);
        maxnum = -1;
        // 策略1：尝试构造位数和 n 一样的数
        dfs(nums, false, 0, s, 0);

        if (maxnum >= n || maxnum == -1)
        {
            int len = s.length() - 1;
            if (len == 0)
                return -1;
            maxnum = 0;
            for (int i = 0; i < len; i++)
            {
                maxnum = maxnum * 10 + nums.back();
            }
        }
    }
};

class Soulution
{
public:
    int maxNumber = -1;
    void dfs(vector<int> &nums, string cur, int target)
    {
        if (!cur.empty())
        {
            int num = stoi(cur);
            if (num < target)
            {
                maxNumber = max(maxNumber, num);
            }
            if (cur.length() >= to_string(target).length())
            {
                return;
            }
        }

        for (auto n : nums)
        {
            dfs(nums, to_string(n) + cur, target);
        }
    }

    int lessn(vector<int> &nums, int target)
    {
        dfs(nums, "", target);
        return maxNumber;
    }
};

int main()
{
    // ACM 模式输入处理
    int m;
    while (cin >> m)
    { // while循环处理多组测试数据（常见ACM套路）
        vector<int> digits(m);
        for (int i = 0; i < m; i++)
        {
            cin >> digits[i];
        }

        int n;
        cin >> n;

        // 调用逻辑
        Soulution s;
        int result = s.lessn(digits, n);
        cout << result << endl;
    }
    return 0;
}