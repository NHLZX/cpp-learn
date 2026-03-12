#include <vector>
#include <iostream>
using namespace std;
#include <string>

#include <sstream>
#include <cmath>

class Buy
{
public:
    Buy() {}
    int Buy_sell(vector<int> &prices)
    {
        int buy1 = -prices[0];
        int buy2 = -prices[0];
        int sell1 = 0;
        int sell2 = 0;
        for (int i = 1; i < prices.size(); i++)
        {
            buy1 = max(buy1, -prices[i]);
            sell1 = max(sell1, buy1 + prices[i]);
            buy2 = max(buy2, sell1 - prices[i]);
            sell2 = max(sell2, buy2 + prices[i]);
        }
        return sell2;
    }
};

int main()
{

    cout << "输入数组" << endl;
    string line;

    while (getline(cin, line))
    {
        if (!line.empty())
        {
            stringstream ss(line);
            vector<int> nums;
            int num;
            while (ss >> num)
            {
                nums.push_back(num);
            }
            Buy buy;
            int sell_max = buy.Buy_sell(nums);
            cout << "最大利润为" << sell_max << endl;
        }
    }

    return 0;
}