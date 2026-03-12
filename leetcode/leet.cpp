#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <queue>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int val1) : val(val1), next(nullptr) {}
    ListNode() : val(0), next(nullptr) {}
    ListNode(int val1, ListNode *next1) : val(val1), next(next1) {}
    /* data */
};

class Solution
{
public:
    ListNode *swapList(ListNode *head)
    {
        ListNode *dummy = new ListNode(0);
        dummy->next = head;
        ListNode *cur = dummy;
        while (cur->next != nullptr && cur->next->next != nullptr)
        {
            ListNode *next = cur->next;
            ListNode *next_next = cur->next->next;
            ListNode *newStart = next_next->next;
            cur->next = next_next;
            next_next->next = next;
            next->next = newStart;
            cur = next;
        }
        ListNode *newHead = dummy->next;
        delete dummy;
        return newHead;
    }

    ListNode *makeListNode(std::vector<int> &nums)
    {
        if (nums.empty())
            return nullptr;
        ListNode *head = new ListNode(nums[0]);
        ListNode *cur = head;
        for (int i = 1; i < nums.size(); i++)
        {
            ListNode *num = new ListNode(nums[i]);
            cur->next = num;
            cur = cur->next;
        }
        return head;
    }

    void printList(ListNode *head)
    {
        while (head)
        {
            std::cout << head->val << " ";
            head = head->next;
        }
        std::cout << std::endl;
    }
};

class Parenthesis
{
public:
    std::vector<std::string> res;
    void dfs(int left, int right, string &temp, int n)
    {
        if (left == n && right == n)
        {
            res.push_back(temp);
        }
        if (left < n)
        {
            temp.push_back('(');
            dfs(left + 1, right, temp, n);
            temp.pop_back();
        }
        if (right < left)
        {
            temp.push_back(')');
            dfs(left, right + 1, temp, n);
            temp.pop_back();
        }
    }
    vector<std::string> gengerator(int n)
    {
        string temp = "";
        dfs(0, 0, temp, n);
        return res;
    }

    void printParent()
    {
        for (auto parent : res)
        {
            cout << parent << " ";
        }
        cout << endl;
    }
};

class MedianFinder
{
public:
    MedianFinder()
    {
    }

    void addNum(int num)
    {
        if (num <= min_que.top() || min_que.empty())
        {
            min_que.push(num);
            if (min_que.size() > max_que.size() + 1)
            {
                max_que.push(min_que.top());
                min_que.pop();
            }
            else
            {
                max_que.push(num);
                if (max_que.size() > min_que.size())
                {
                    min_que.push(max_que.top());
                    max_que.pop();
                }
            }
        }
    }

    double findMedian()
    {
        if (min_que.size() > max_que.size())
        {
            return min_que.top();
        }
        return (min_que.top() + max_que.top()) / 2.0;
    }
    std::priority_queue<int, std::vector<int>, std::less<int>> min_que;
    std::priority_queue<int, std::vector<int>, std::greater<int>> max_que;
};

struct Minnum
{
    int score;
    std::string name;
    // bool operator <(const Minnum& num) const{
    //     return this->score < num.score;
    // }
};
struct Compare
{
    bool operator()(const Minnum &num1, const Minnum &num2)
    {
        return num1.score > num2.score;
    }
};

int main()
{
    int num_pair;
    cout << "请输入括号的数量,不为0,输入0进入下一题" << endl;
    while (cin >> num_pair)
    {
        Parenthesis p;
        p.gengerator(num_pair);
        p.printParent();
    }

    std::string line;
    cout << "两两交换链表" << endl;
    while (getline(std::cin, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        int val;
        std::vector<int> nums;
        while (ss >> val)
        {
            nums.push_back(val);
        }
        Solution sol;

        ListNode *head = sol.makeListNode(nums);
        ListNode *newHead = sol.swapList(head);
        sol.printList(newHead);
    }
    return 0;
}