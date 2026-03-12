#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <unistd.h>      // 用于 getpid()
#include <sys/syscall.h> // 用于 syscall()
#include <cstring>       // 用于 string操作

// 全局互斥锁，防止打印输出乱序
std::mutex print_mtx;

// 【核心函数】获取内核视角的真实 TID (LWP ID)
pid_t get_core_tid()
{
    // glibc 没有直接封装 gettid，必须手动调用 syscall
    return syscall(SYS_gettid);
}

// 线程的工作函数
void worker(int thread_num)
{
    // 为了防止多个线程同时打印导致乱码，加个锁
    {
        std::lock_guard<std::mutex> lock(print_mtx);

        // 1. 获取 PID (进程 ID - 所有线程应该一样)
        pid_t pid = getpid();

        // 2. 获取 TID (内核线程 ID - 每个线程唯一)
        pid_t tid = get_core_tid();

        // 3. 获取 pthread_t (C++库句柄 - 是一串内存地址)
        std::thread::id cpp_id = std::this_thread::get_id();

        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "[子线程 " << thread_num << "] 启动成功!" << std::endl;
        std::cout << " > 进程 ID (PID/TGID) : " << pid << " (大家共享)" << std::endl;
        std::cout << " > 内核 ID (TID/LWP)  : " << tid << " (系统唯一)" << std::endl;
        std::cout << " > C++ 句柄 (Handle)  : " << cpp_id << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
    }

    // 模拟长时间工作，让线程挂着不退出
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(100));
    }
}

int main()
{
    std::cout << "================== 主程序开始 ==================" << std::endl;

    // 打印主线程的信息
    {
        std::lock_guard<std::mutex> lock(print_mtx);
        std::cout << "[主线程 Main] 正在运行..." << std::endl;
        std::cout << " > 进程 ID (PID)      : " << getpid() << std::endl;
        std::cout << " > 内核 ID (TID)      : " << get_core_tid() << " (应该等于 PID)" << std::endl;
        std::cout << " > C++ 句柄           : " << std::this_thread::get_id() << std::endl;
    }

    // 启动 3 个子线程
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; ++i)
    {
        // 创建线程，传入 worker 函数和编号
        threads.emplace_back(worker, i);
        // 稍微停顿一下，让输出顺序好看点
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\n所有线程已启动。正在挂起，请去终端查看 ps -eLf ..." << std::endl;

    // join() 会阻塞主线程，等待子线程结束
    // 因为子线程里是死循环，所以主程序会一直挂着
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    return 0;
}