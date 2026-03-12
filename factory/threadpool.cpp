#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>

#include <chrono>
class MyThreadPool
{
public:
    MyThreadPool(size_t nums) : isstop(false)
    {
        // 向workers放入任务 lamda表达式
        for (size_t i = 0; i < nums; i++)
        {
            workers.emplace_back([this]()
                                 {
                                     while (true)
                                     {
                                         std::function<void()> task;
                                         {
                                             std::unique_lock<std::mutex> lock(this->mtx);
                                             cond.wait(lock,[this](){
                                                return this->isstop || this->que_mut.empty();
                                             });
                                             if(this->isstop && this->que_mut.empty())
                                                        return;
                                             task = std::move(this->que_mut.front());
                                             this->que_mut.pop();
                                         }
                                         task();
                                     } });
        }
    }
    template <class F, class... Args>
    void addTask(F &&f, Args &&...arg)
    {
        std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Arg>(arg)...);
        {
            std::unique_lock<std::mutex> lock(mtx);
            if (isstop)
            {
                return;
            }
            que_mut.push(std::move(task));
        }
        cond.notify_one();
    }

private:
    std::queue<std::function<void()>> que_mut;
    std::mutex mtx;
    std::vector<std::thread> workers;
    bool isstop;
    std::condition_variable cond;
};

class ThreadPool
{
public:
    ThreadPool(size_t threads) : stop(false)
    {
        for (size_t i = 0; i < threads; ++i)
        {
            workers.emplace_back([this]
                                 {
                for(;;) {
                    std::function<void()> task;
                    {
                        // 1. 获取锁
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        
                        // 2. 等待条件满足：停止了 或 有新任务
                        this->condition.wait(lock, 
                            [this](){ return this->stop || !this->tasks.empty(); });
                        
                        // 3. 如果是停止且没任务，退出线程
                        if(this->stop && this->tasks.empty())
                            return;
                        
                        // 4. 取出任务
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    // 5. 执行任务
                    task();
                } });
        }
    }
    template <class F, class... Args>
    void enqueue(F &&f, Args &&...args)
    {
        std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            if (stop)
                return;
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
        {
            if (worker.joinable())
                worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    // 任务队列
    std::queue<std::function<void()>> tasks;
    // 同步工具
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

void print_hello(int i)
{
    std::cout << "Task " << i << " is running on thread "
              << std::this_thread::get_id() << std::endl;
}

struct MyClass
{
    void run(int i)
    {
        std::cout << "Class Method " << i << std::endl;
    }
};

int main()
{
    ThreadPool pool(4);

    // 1. 提交普通函数
    for (int i = 0; i < 5; ++i)
    {
        pool.enqueue(print_hello, i);
    }

    // 2. 提交 Lambda
    pool.enqueue([]
                 { std::cout << "Lambda task is running!" << std::endl; });

    // 3. 提交类成员函数
    MyClass obj;
    pool.enqueue(&MyClass::run, &obj, 999);

    // 等一会，让任务跑完再退出，否则 main 结束线程池就销毁了
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}