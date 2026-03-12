#include <iostream>
#include <vector>
using namespace std;
int coinChange(vector<int> &coins, int amount)
{
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int i = 1; i < amount; i++)
    {
        for (auto coin : coins)
        {
            if (coin <= amount)
            {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}
int main()
{
    return 0;
}