#include <iostream>
using namespace std;
#include <memory>
#include <string>

class Element
{
public:
    std::string name;
    Element(std::string n) : name(n)
    {
        std::cout << "构造: " << name << " @" << this << std::endl;
    }
    ~Element()
    {
        std::cout << "析构: " << name << " @" << this << std::endl;
    }
    void sayHello()
    {
        std::cout << "Hello, I am " << name << std::endl;
    }
};

class Parent;

class Child
{
public:
    Child(string name) : child_name(name)
    {
        cout << child_name << endl;
    }
    ~Child() {}
    shared_ptr<Parent> parent;
    string child_name;
};

class Parent
{
public:
    Parent(string name) : parent_name(name)
    {
        cout << parent_name << endl;
    }
    ~Parent() { std::cout << "Parent析构\n"; }
    weak_ptr<Child> child;

    string parent_name;
};

void weak_ptr_demo()
{
    std::cout << "\n--- weak_ptr 解决循环引用 ---" << std::endl;

    auto dad = std::make_shared<Parent>("Dad");
    auto son = std::make_shared<Child>("Son");

    // 建立关系
    dad->child = son;  // weak_ptr 赋值
    son->parent = dad; // shared_ptr 赋值

    std::cout << "dad 引用计数: " << dad.use_count() << std::endl; // 2 (dad + son持有)
    std::cout << "son 引用计数: " << son.use_count() << std::endl; // 1 (只有 son 持有，dad 是 weak)

    // 如何使用 weak_ptr？必须先 lock()
    if (auto son_sp = dad->child.lock())
    { // lock() 尝试升级为 shared_ptr
        std::cout << "通过 weak_ptr 访问到了: " << son_sp->child_name << std::endl;
    }
    else
    {
        std::cout << "对象已经不存在了" << std::endl;
    }
}

void unique_ptr_demo()
{
    std::unique_ptr<Element> p1 = make_unique<Element>("name");

    p1->sayHello();
    std::unique_ptr<Element> p2 = std::move(p1);
    p2->sayHello();
    if (p1 == nullptr)
    {
        std::cout << "p1 已经是空指针了" << std::endl;
    }
    if (p2 != nullptr)
    {
        std::cout << "p2 现在拥有对象 A" << std::endl;
    }

    // 5. 作用域结束，p2 销毁，自动 delete 内存
    std::cout << "函数结束" << std::endl;

    // 5. 作用域结束，p2 销毁，自动 delete 内存
    std::cout << "函数结束" << std::endl;
}

int main()
{

    int x = 0;
    int const &i = x;

    auto a = i;
    decltype(x) b = i;
    shared_ptr<int> p = make_shared<int>(1);

    unique_ptr_demo();
    return 0;
}