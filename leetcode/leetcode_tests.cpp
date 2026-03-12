// leetcode_tests.cpp
// 对 leetcode 目录下所有可测试函数的统一测试用例
// 编译方法: g++ -std=c++17 -o leetcode_tests leetcode_tests.cpp && ./leetcode_tests

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// ============================================================
//  测试统计辅助
// ============================================================
static int g_pass = 0;
static int g_fail = 0;

// 通用断言：比较两个可打印值
template <typename T>
void CHECK(const char* caseName, T actual, T expected) {
    if (actual == expected) {
        cout << "  [PASS] " << caseName << endl;
        ++g_pass;
    } else {
        cout << "  [FAIL] " << caseName
             << "  期望=" << expected << "  实际=" << actual << endl;
        ++g_fail;
    }
}

// 布尔断言
void CHECK_BOOL(const char* caseName, bool cond) {
    if (cond) {
        cout << "  [PASS] " << caseName << endl;
        ++g_pass;
    } else {
        cout << "  [FAIL] " << caseName << endl;
        ++g_fail;
    }
}

// 打印分隔线
void SECTION(const char* title) {
    cout << "\n==============================\n";
    cout << " " << title << "\n";
    cout << "==============================\n";
}

// ============================================================
//  leetcode11.cpp — 盛最多水的容器（双指针）
// ============================================================
int maxArea(vector<int>& height) {
    int maxarea = 0;
    int left = 0;
    int right = height.size() - 1;
    while (left < right) {
        if (height[left] < height[right]) {
            maxarea = max(maxarea, height[left] * (right - left));
            left++;
        } else {
            maxarea = max(maxarea, height[right] * (right - left));
            right--;
        }
    }
    return maxarea;
}

void test_maxArea() {
    SECTION("leetcode11 - 盛最多水的容器");
    // 标准用例：[1,8,6,2,5,4,8,3,7] → 49（左1右7，宽7，高7）
    vector<int> h1 = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    CHECK("标准用例 [1,8,6,2,5,4,8,3,7]", maxArea(h1), 49);

    // 仅两个元素，取min(1,1)*1=1
    vector<int> h2 = {1, 1};
    CHECK("两元素相等 [1,1]", maxArea(h2), 1);

    // 递减序列：最优为首尾，min(4,4)*4=16
    vector<int> h3 = {4, 3, 2, 1, 4};
    CHECK("首尾相等 [4,3,2,1,4]", maxArea(h3), 16);

    // [1,2,1]：最优为0和2，min(1,1)*2=2
    vector<int> h4 = {1, 2, 1};
    CHECK("三元素 [1,2,1]", maxArea(h4), 2);

    // 全部相同高度
    vector<int> h5 = {3, 3, 3, 3};
    CHECK("全相同高度 [3,3,3,3]", maxArea(h5), 9);
}

// ============================================================
//  leetcode122.cpp — 买卖股票最佳时机（可无限次交易）
// ============================================================
class Buy {
public:
    int Buy_sell(vector<int>& prices) {
        int buy1 = -prices[0];
        int buy2 = -prices[0];
        int sell1 = 0;
        int sell2 = 0;
        for (int i = 1; i < (int)prices.size(); i++) {
            buy1  = max(buy1,  -prices[i]);
            sell1 = max(sell1, buy1 + prices[i]);
            buy2  = max(buy2,  sell1 - prices[i]);
            sell2 = max(sell2, buy2 + prices[i]);
        }
        return sell2;
    }
};

void test_buy_sell() {
    SECTION("leetcode122 - 买卖股票最佳时机（无限次交易）");
    Buy b;

    // [7,1,5,3,6,4]：买1卖5赚4，买3卖6赚3，共7
    vector<int> p1 = {7, 1, 5, 3, 6, 4};
    CHECK("标准用例 [7,1,5,3,6,4]", b.Buy_sell(p1), 7);

    // [1,2,3,4,5]：每天买卖，总利润4
    vector<int> p2 = {1, 2, 3, 4, 5};
    CHECK("单调递增 [1,2,3,4,5]", b.Buy_sell(p2), 4);

    // [7,6,4,3,1]：单调递减，利润为0
    vector<int> p3 = {7, 6, 4, 3, 1};
    CHECK("单调递减 [7,6,4,3,1]", b.Buy_sell(p3), 0);

    // [1,1,1,1]：价格不变，利润为0
    vector<int> p4 = {1, 1, 1, 1};
    CHECK("价格不变 [1,1,1,1]", b.Buy_sell(p4), 0);
}

// ============================================================
//  leetcode128.cpp — 最长连续序列
// ============================================================
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> s;
    for (auto num : nums) s.insert(num);
    int longest = 0;
    for (auto n : s) {
        if (!s.count(n - 1)) {
            int count = 1, cur = n;
            while (s.count(cur + 1)) { cur++; count++; }
            longest = max(longest, count);
        }
    }
    return longest;
}

void test_longestConsecutive() {
    SECTION("leetcode128 - 最长连续序列");

    // [100,4,200,1,3,2] → 序列1,2,3,4，长度4
    vector<int> n1 = {100, 4, 200, 1, 3, 2};
    CHECK("标准用例 [100,4,200,1,3,2]", longestConsecutive(n1), 4);

    // [0,3,7,2,5,8,4,6,0,1] → 0..8，长度9
    vector<int> n2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    CHECK("含重复元素 [0..8含重复]", longestConsecutive(n2), 9);

    // 空数组 → 0
    vector<int> n3 = {};
    CHECK("空数组", longestConsecutive(n3), 0);

    // 单元素 → 1
    vector<int> n4 = {42};
    CHECK("单元素 [42]", longestConsecutive(n4), 1);

    // 全相同 → 1
    vector<int> n5 = {5, 5, 5, 5};
    CHECK("全相同 [5,5,5,5]", longestConsecutive(n5), 1);
}

// ============================================================
//  leetcode136.cpp — 寻找出现两次的数（其余出现三次）
//
//  ⚠ findtwonums2 存在两个bug：
//    1. return ans 在外层 for 循环体内，仅执行 i=0 就返回
//    2. count % 3 == 2 的判断在内层循环中，逐元素判断而非全部统计后判断
//  本测试仅验证正确实现 findtwonums。
// ============================================================
int findtwonums(vector<int>& nums) {
    int ans = 0;
    for (int i = 0; i < 32; i++) {
        int count = 0;
        for (auto num : nums) {
            if ((num >> i) & 1) count++;
        }
        if (count % 3 == 2) ans |= 1 << i;
    }
    return ans;
}

void test_findtwonums() {
    SECTION("leetcode136 - 寻找出现两次的数（其余出现三次）");

    // {2,2,2,3,3,4,4,4,5,5,5}：3出现两次，其他三次 → 3
    vector<int> n1 = {2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5};
    CHECK("标准用例 → 3", findtwonums(n1), 3);

    // {1,1,2,2,2}：1出现两次 → 1
    vector<int> n2 = {1, 1, 2, 2, 2};
    CHECK("出现两次的是1", findtwonums(n2), 1);

    // {7,7,7,9,9}：9出现两次 → 9
    vector<int> n3 = {7, 7, 7, 9, 9};
    CHECK("出现两次的是9", findtwonums(n3), 9);
}

// ============================================================
//  leetcode279.cpp — 完全平方数（最少数量）
//
//  ⚠ 原始实现存在 bug：内层循环条件为 j*j <= num（总范围）
//    而非 j*j <= i（当前子问题范围），当 i < j*j 时访问负索引，
//    导致未定义行为（UB）。
//  本测试仅验证不触发 UB 的安全输入（num ≤ 3，此时 j 最大为1）。
// ============================================================
int numsqrt(int num) {
    vector<int> f(num + 1);
    for (int i = 1; i <= num; i++) {
        int num_min = INT_MAX;
        for (int j = 1; j * j <= num; j++) {
            num_min = min(num_min, f[i - j * j]);
        }
        f[i] = num_min + 1;
    }
    return f[num];
}

void test_numsqrt() {
    SECTION("leetcode279 - 完全平方数（最少数量）");
    cout << "  [NOTE] 原实现 bug：j*j<=num 应为 j*j<=i，num>=4 时访问负索引（UB）\n";
    cout << "  [NOTE] 以下仅测试 num<=3 的安全用例\n";

    // 1 = 1²，最少1个
    CHECK("numsqrt(1) = 1", numsqrt(1), 1);
    // 2 = 1+1，最少2个
    CHECK("numsqrt(2) = 2", numsqrt(2), 2);
    // 3 = 1+1+1，最少3个
    CHECK("numsqrt(3) = 3", numsqrt(3), 3);
}

// ============================================================
//  leetcode300.cpp — 最长递增子序列（LIS）
//
//  ⚠ 存在 bug：num_max 仅在内层循环找到 nums[i]>nums[j] 时更新，
//    若数组长度为1或所有元素相等，内层循环永不触发，返回0而非1。
// ============================================================
int lengthOfLIS(vector<int>& nums) {
    vector<int> dp(nums.size());
    int num_max = 0;
    for (int i = 0; i < (int)nums.size(); i++) {
        dp[i] = 1;
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                dp[i]   = max(dp[i],   dp[j] + 1);
                num_max = max(dp[i],   num_max);
            }
        }
    }
    return num_max;
}

void test_lengthOfLIS() {
    SECTION("leetcode300 - 最长递增子序列");

    // 标准用例 → 4（2,3,7,101 或 2,5,7,101）
    vector<int> n1 = {10, 9, 2, 5, 3, 7, 101, 18};
    CHECK("标准用例 [10,9,2,5,3,7,101,18]", lengthOfLIS(n1), 4);

    // [0,1,0,3,2,3] → 4（0,1,2,3）
    vector<int> n2 = {0, 1, 0, 3, 2, 3};
    CHECK("[0,1,0,3,2,3] → 4", lengthOfLIS(n2), 4);

    // 单调递增 → 5
    vector<int> n3 = {1, 3, 6, 7, 9};
    CHECK("单调递增 [1,3,6,7,9] → 5", lengthOfLIS(n3), 5);

    // ⚠ bug 触发：单元素应返回1，实际返回0
    vector<int> n4 = {1};
    cout << "  [NOTE] 以下两项预期触发 bug（期望1，实际返回0）\n";
    CHECK("单元素 [1] → 期望1（bug返回0）", lengthOfLIS(n4), 1);

    // ⚠ bug 触发：全相同元素应返回1，实际返回0
    vector<int> n5 = {7, 7, 7, 7, 7};
    CHECK("全相同 [7,7,7,7,7] → 期望1（bug返回0）", lengthOfLIS(n5), 1);
}

// ============================================================
//  leetcode322.cpp — 零钱兑换（动态规划）
//
//  ⚠ 存在两个 bug：
//    1. 外层循环为 i < amount，应为 i <= amount，
//       dp[amount] 从未被更新，始终返回 -1
//    2. 内层条件 coin <= amount 应为 coin <= i，
//       可能访问 dp[i-coin] 为负索引（当 coin > i 时）
// ============================================================
int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int i = 1; i < amount; i++) {        // bug: 应为 i <= amount
        for (auto coin : coins) {
            if (coin <= amount) {              // bug: 应为 coin <= i
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

void test_coinChange() {
    SECTION("leetcode322 - 零钱兑换");
    cout << "  [NOTE] 原实现 bug：循环为 i<amount 应为 i<=amount，dp[amount]永不更新\n";
    cout << "  [NOTE] 以下用例预期全部 FAIL（bug导致错误返回-1）\n";

    // [1,2,5], amount=11 → 3（5+5+1），bug返回-1
    vector<int> c1 = {1, 2, 5};
    CHECK("[1,2,5] amount=11 → 期望3（bug返回-1）", coinChange(c1, 11), 3);

    // [2], amount=4 → 2（2+2），bug返回-1
    vector<int> c2 = {2};
    CHECK("[2] amount=4 → 期望2（bug返回-1）", coinChange(c2, 4), 2);

    // amount=0 → 0（不需要任何硬币），此用例无需循环，正确
    vector<int> c3 = {1, 5};
    CHECK("[1,5] amount=0 → 0（无bug）", coinChange(c3, 0), 0);

    // 不可凑成 → -1，[3] amount=11 → bug也返回-1（凑巧正确）
    vector<int> c4 = {3};
    CHECK("[3] amount=11 → -1（不可凑，与bug结果一致）", coinChange(c4, 11), -1);
}

// ============================================================
//  leetcode45.cpp — 跳跃游戏 II（最少跳跃次数）
// ============================================================
int jump(vector<int>& nums) {
    int jump_num = 0, end = 0, max_num = 0;
    for (int i = 0; i < (int)nums.size() - 1; i++) {
        if (max_num >= i) {
            max_num = max(max_num, i + nums[i]);
            if (i == end) { jump_num++; end = max_num; }
        }
    }
    return jump_num;
}

void test_jump() {
    SECTION("leetcode45 - 跳跃游戏 II（最少跳跃次数）");

    // [2,3,1,1,4] → 2（0→1→4）
    vector<int> n1 = {2, 3, 1, 1, 4};
    CHECK("[2,3,1,1,4] → 2", jump(n1), 2);

    // [2,3,0,1,4] → 2（0→1→4）
    vector<int> n2 = {2, 3, 0, 1, 4};
    CHECK("[2,3,0,1,4] → 2", jump(n2), 2);

    // 单元素，已在终点 → 0
    vector<int> n3 = {0};
    CHECK("单元素 [0] → 0", jump(n3), 0);

    // [1,1,1,1] → 3（每次只能跳1步）
    vector<int> n4 = {1, 1, 1, 1};
    CHECK("[1,1,1,1] → 3", jump(n4), 3);

    // [5,1,1,1,1] → 1（一步跳到末尾）
    vector<int> n5 = {5, 1, 1, 1, 1};
    CHECK("[5,1,1,1,1] → 1", jump(n5), 1);
}

// ============================================================
//  leetcode49.cpp — 字母异位词分组
// ============================================================
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> mp;
    for (auto s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        mp[key].emplace_back(s);
    }
    vector<vector<string>> ans;
    for (const auto& [_, v] : mp) ans.emplace_back(v);
    return ans;
}

// 比较两个分组结果是否相同（忽略组内及组间顺序）
bool sameGroups(vector<vector<string>> a, vector<vector<string>> b) {
    for (auto& v : a) sort(v.begin(), v.end());
    for (auto& v : b) sort(v.begin(), v.end());
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    return a == b;
}

void test_groupAnagrams() {
    SECTION("leetcode49 - 字母异位词分组");

    // 标准用例
    vector<string> s1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    auto r1 = groupAnagrams(s1);
    vector<vector<string>> e1 = {{"eat","tea","ate"}, {"tan","nat"}, {"bat"}};
    CHECK_BOOL("标准用例 [eat,tea,tan,ate,nat,bat]", sameGroups(r1, e1));

    // 单空字符串
    vector<string> s2 = {""};
    auto r2 = groupAnagrams(s2);
    vector<vector<string>> e2 = {{""}};
    CHECK_BOOL("单空字符串 [\"\"]", sameGroups(r2, e2));

    // 单字符串
    vector<string> s3 = {"a"};
    auto r3 = groupAnagrams(s3);
    vector<vector<string>> e3 = {{"a"}};
    CHECK_BOOL("单字符串 [\"a\"]", sameGroups(r3, e3));

    // 全是同一异位词组
    vector<string> s4 = {"abc", "bca", "cab"};
    auto r4 = groupAnagrams(s4);
    vector<vector<string>> e4 = {{"abc","bca","cab"}};
    CHECK_BOOL("全同异位词 [abc,bca,cab]", sameGroups(r4, e4));
}

// ============================================================
//  leet.cpp — Solution::swapList（两两交换链表节点）
// ============================================================
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

class Solution {
public:
    ListNode* swapList(ListNode* head) {
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        ListNode* cur = dummy;
        while (cur->next && cur->next->next) {
            ListNode* a  = cur->next;
            ListNode* b  = cur->next->next;
            cur->next    = b;
            a->next      = b->next;
            b->next      = a;
            cur          = a;
        }
        ListNode* newHead = dummy->next;
        delete dummy;
        return newHead;
    }

    ListNode* makeList(vector<int> nums) {
        if (nums.empty()) return nullptr;
        ListNode* head = new ListNode(nums[0]);
        ListNode* cur = head;
        for (int i = 1; i < (int)nums.size(); i++) {
            cur->next = new ListNode(nums[i]);
            cur = cur->next;
        }
        return head;
    }

    vector<int> toVec(ListNode* head) {
        vector<int> v;
        while (head) { v.push_back(head->val); head = head->next; }
        return v;
    }

    void freeList(ListNode* head) {
        while (head) { auto* t = head->next; delete head; head = t; }
    }
};

void test_swapList() {
    SECTION("leet.cpp - 两两交换链表节点");
    Solution sol;

    // [1,2,3,4] → [2,1,4,3]
    auto* h1 = sol.makeList({1, 2, 3, 4});
    CHECK_BOOL("[1,2,3,4] → [2,1,4,3]",
               sol.toVec(sol.swapList(h1)) == vector<int>({2, 1, 4, 3}));

    // [1,2,3]（奇数个）→ [2,1,3]
    auto* h2 = sol.makeList({1, 2, 3});
    CHECK_BOOL("[1,2,3] → [2,1,3]",
               sol.toVec(sol.swapList(h2)) == vector<int>({2, 1, 3}));

    // 单节点 → [1]
    auto* h3 = sol.makeList({1});
    CHECK_BOOL("单节点 [1] → [1]",
               sol.toVec(sol.swapList(h3)) == vector<int>({1}));

    // 空链表 → 空
    CHECK_BOOL("空链表 → 空",
               sol.toVec(sol.swapList(nullptr)) == vector<int>());
}

// ============================================================
//  leet.cpp — Parenthesis::gengerator（生成所有有效括号组合）
// ============================================================
class Parenthesis {
public:
    vector<string> res;
    void dfs(int left, int right, string& temp, int n) {
        if (left == n && right == n) { res.push_back(temp); return; }
        if (left < n) {
            temp.push_back('(');
            dfs(left + 1, right, temp, n);
            temp.pop_back();
        }
        if (right < left) {
            temp.push_back(')');
            dfs(left, right + 1, temp, n);
            temp.pop_back();
        }
    }
    vector<string> gengerator(int n) {
        res.clear();
        string temp;
        dfs(0, 0, temp, n);
        return res;
    }
};

void test_parenthesis() {
    SECTION("leet.cpp - 生成所有有效括号组合");

    // n=1 → ["()"]
    Parenthesis p;
    auto r1 = p.gengerator(1);
    sort(r1.begin(), r1.end());
    vector<string> e1 = {"()"};
    CHECK_BOOL("n=1 → [\"()\"]", r1 == e1);

    // n=2 → ["(())", "()()"]
    auto r2 = p.gengerator(2);
    sort(r2.begin(), r2.end());
    vector<string> e2 = {"(())", "()()"};
    sort(e2.begin(), e2.end());
    CHECK_BOOL("n=2 → [\"(())\",\"()()\"]", r2 == e2);

    // n=3 → 5种组合
    auto r3 = p.gengerator(3);
    CHECK("n=3 结果数量=5", (int)r3.size(), 5);

    // 验证n=3结果都是有效括号串（每个左括号都有对应右括号）
    bool valid = true;
    for (auto& s : r3) {
        int bal = 0;
        for (char c : s) {
            if (c == '(') bal++;
            else bal--;
            if (bal < 0) { valid = false; break; }
        }
        if (bal != 0) { valid = false; break; }
    }
    CHECK_BOOL("n=3 所有结果均为有效括号串", valid);
}

// ============================================================
//  leet.cpp — MedianFinder（数据流中位数）
//
//  ⚠ 存在 bug：addNum 中条件为
//      if (num <= min_que.top() || min_que.empty())
//    应先判空再调用 top()：
//      if (min_que.empty() || num <= min_que.top())
//    第一次调用 addNum 时 min_que 为空，调用 top() 为未定义行为（UB）。
//    本测试跳过实际调用，仅记录 bug 说明。
// ============================================================
void test_medianFinder() {
    SECTION("leet.cpp - MedianFinder（数据流中位数）");
    cout << "  [SKIP] 存在 bug：addNum 首次调用时对空堆调用 top() 导致 UB\n";
    cout << "  [SKIP] 修复方式：将条件改为 if (min_que.empty() || num <= min_que.top())\n";
}

// ============================================================
//  lessn.cpp — Soulution::lessn（用给定数字集合构造小于n的最大数）
//
//  注：文件中有两个类：
//    Soultion（第一个）：lessn 末尾缺少 return 语句，为 UB
//    Soulution（第二个）：正确实现，main() 中使用的也是此类
//  本测试使用 Soulution（正确版本）。
// ============================================================
class Soulution {
public:
    int maxNumber = -1;

    void dfs(vector<int>& nums, string cur, int target) {
        if (!cur.empty()) {
            int num = stoi(cur);
            if (num < target) maxNumber = max(maxNumber, num);
            if ((int)cur.length() >= (int)to_string(target).length()) return;
        }
        for (auto n : nums) {
            dfs(nums, to_string(n) + cur, target);
        }
    }

    int lessn(vector<int>& nums, int target) {
        maxNumber = -1;
        dfs(nums, "", target);
        return maxNumber;
    }
};

void test_lessn() {
    SECTION("lessn.cpp - 用给定数字集合构造严格小于n的最大整数");

    // digits=[1,2], n=23 → 22（22 < 23，最大）
    Soulution s1;
    vector<int> d1 = {1, 2};
    CHECK("digits=[1,2] n=23 → 22", s1.lessn(d1, 23), 22);

    // digits=[1,2], n=20 → 12（12 < 20，最大两位数）
    Soulution s2;
    CHECK("digits=[1,2] n=20 → 12", s2.lessn(d1, 20), 12);

    // digits=[2,3], n=33 → 32（32 < 33）
    Soulution s3;
    vector<int> d2 = {2, 3};
    CHECK("digits=[2,3] n=33 → 32", s3.lessn(d2, 33), 32);

    // digits=[1,3,5,7], n=100 → 77（77 < 100，两位最大）
    Soulution s4;
    vector<int> d3 = {1, 3, 5, 7};
    CHECK("digits=[1,3,5,7] n=100 → 77", s4.lessn(d3, 100), 77);

    // digits=[5], n=5 → -1（无法构造严格小于5的正整数，仅能构造5,55,...）
    Soulution s5;
    vector<int> d4 = {5};
    CHECK("digits=[5] n=5 → -1（不存在）", s5.lessn(d4, 5), -1);
}

// ============================================================
//  alloctor.cpp — MemoryPool（固定对象大小的内存池）
// ============================================================
class MemoryPool {
public:
    MemoryPool(size_t objectSize, size_t poolSize)
        : objSize(objectSize), pool((char*)malloc(objectSize * poolSize)) {
        if (!pool) throw std::bad_alloc();
        for (size_t i = 0; i < poolSize; ++i)
            freeList.push(pool + i * objectSize);
    }
    ~MemoryPool() { free(pool); }

    void* allocate() {
        if (freeList.empty()) throw std::bad_alloc();
        void* ptr = freeList.top();
        freeList.pop();
        return ptr;
    }

    void deallocate(void* ptr) { freeList.push((char*)ptr); }

    size_t available() const { return freeList.size(); }

private:
    size_t objSize;
    char* pool;
    stack<void*> freeList;
};

struct TestObj {
    int value;
    TestObj(int v) : value(v) {}
};

void test_memoryPool() {
    SECTION("alloctor.cpp - MemoryPool（内存池）");

    // 基本分配与使用
    MemoryPool pool(sizeof(TestObj), 3);
    CHECK("初始可用槽位=3", (int)pool.available(), 3);

    void* m1 = pool.allocate();
    CHECK("分配1次后可用槽位=2", (int)pool.available(), 2);

    // 使用定位 new 构造对象
    TestObj* obj1 = new (m1) TestObj(42);
    CHECK("定位new构造值=42", obj1->value, 42);

    // 分配第二个槽位
    void* m2 = pool.allocate();
    TestObj* obj2 = new (m2) TestObj(99);
    CHECK("第二个对象值=99", obj2->value, 99);
    CHECK("分配2次后可用槽位=1", (int)pool.available(), 1);

    // 释放并重新分配
    obj1->~TestObj();
    pool.deallocate(m1);
    CHECK("释放1次后可用槽位=2", (int)pool.available(), 2);

    void* m3 = pool.allocate();
    TestObj* obj3 = new (m3) TestObj(77);
    CHECK("重用槽位构造值=77", obj3->value, 77);

    // 耗尽内存池后抛出 bad_alloc
    pool.allocate();  // 使用最后一个槽
    bool threw = false;
    try {
        pool.allocate();  // 应抛出 bad_alloc
    } catch (const std::bad_alloc&) {
        threw = true;
    }
    CHECK_BOOL("池耗尽时抛出 bad_alloc", threw);

    // 清理
    obj2->~TestObj();
    obj3->~TestObj();
}

// ============================================================
//  main：运行所有测试并汇总结果
// ============================================================
int main() {
    cout << "========================================\n";
    cout << "   LeetCode 测试套件\n";
    cout << "========================================\n";

    test_maxArea();
    test_buy_sell();
    test_longestConsecutive();
    test_findtwonums();
    test_numsqrt();
    test_lengthOfLIS();
    test_coinChange();
    test_jump();
    test_groupAnagrams();
    test_swapList();
    test_parenthesis();
    test_medianFinder();
    test_lessn();
    test_memoryPool();

    cout << "\n========================================\n";
    cout << "  汇总：PASS=" << g_pass << "  FAIL=" << g_fail << "\n";
    cout << "========================================\n";

    return g_fail > 0 ? 1 : 0;
}
