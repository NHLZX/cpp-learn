#include <iostream>
#include <cstdlib>
#include <stack>

class MemoryPool
{
public:
    MemoryPool(size_t objectSize, size_t poolSize)
        : objSize(objectSize), totalSize(poolSize), pool((char *)malloc(objectSize * poolSize))
    {
        if (pool == nullptr)
        {
            throw std::bad_alloc();
        }
        // 初始化 free list
        for (size_t i = 0; i < poolSize; ++i)
        {
            freeList.push(pool + i * objectSize);
        }
    }

    ~MemoryPool()
    {
        free(pool);
    }

    void *allocate()
    {
        if (freeList.empty())
        {
            throw std::bad_alloc();
        }
        void *ptr = freeList.top();
        freeList.pop();
        return ptr;
    }

    void deallocate(void *ptr)
    {
        freeList.push((char *)ptr);
    }

private:
    size_t objSize;
    size_t totalSize;
    char *pool;
    std::stack<void *> freeList;
};

class MyClass
{
public:
    MyClass(int val) : value(val)
    {
        std::cout << "MyClass constructor: " << value << std::endl;
    }
    ~MyClass()
    {
        std::cout << "MyClass destructor: " << value << std::endl;
    }
    int value;
};

int main()
{
    try
    {
        // 创建一个能容纳 3 个 MyClass 对象的内存池
        MemoryPool pool(sizeof(MyClass), 3);

        // 分配对象内存
        void *mem1 = pool.allocate();
        void *mem2 = pool.allocate();

        // 使用“定位 new”构造对象
        MyClass *obj1 = new (mem1) MyClass(100);
        MyClass *obj2 = new (mem2) MyClass(200);

        // 使用对象
        std::cout << "obj1 value: " << obj1->value << std::endl;
        std::cout << "obj2 value: " << obj2->value << std::endl;

        // 显式调用析构函数
        obj1->~MyClass();
        obj2->~MyClass();

        // 释放内存
        pool.deallocate(mem1);
        pool.deallocate(mem2);
    }
    catch (const std::bad_alloc &e)
    {
        std::cerr << "Memory pool allocation error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}