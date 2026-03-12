#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;
using namespace std::placeholders;
void printhello(string name)
{
    cout << "print" << name << endl;
}

struct Functor
{
    void operator()(string name)
    {
        cout << "Fun: " << name << endl;
    }
};

void addThree(int a, int b, int c)
{
    cout << a << "+" << b << "+" << c << "=" << a + b + c << endl;
}

int main()
{
    function<void(string)> func_wrapper;
    func_wrapper = printhello;
    func_wrapper("lzx");
    func_wrapper = [](string name)
    {
        cout << "lamda:hello" << endl;
    };
    func_wrapper("lzx1");
    Functor F;
    func_wrapper = F;
    func_wrapper("lyy");
    auto newFunc = bind(addThree, _1, 20, 30);
    newFunc(100);
    auto crazyFunc = bind(addThree, _2, _1, 300);
    crazyFunc(100, 200);

    return 0;
}