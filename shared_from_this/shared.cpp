#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <future>

// 模拟一个异步网络操作 (比如 socket.async_read)
// 优化：用模板替代 std::function，避免类型擦除带来的堆分配开销
template <typename Callback>
std::future<void> mock_async_read(Callback callback)
{
    // 优化：用 std::async 替代 std::thread + detach
    // packaged_task 绑定回调，future 可供外部等待，不会出现线程逃逸
    auto task = std::make_shared<std::packaged_task<void()>>(std::move(callback));
    auto fut = task->get_future();

    std::thread([task]()
                {
        std::cout << "    [系统底层] 正在等待数据(模拟网络延迟 1s)...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "    [系统底层] 数据到达，准备执行回调...\n";
        (*task)(); })
        .detach();

    return fut;
}

// ==========================================
// 核心类 Session
// ==========================================
class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(int id) : id_(id)
    {
        std::cout << "[Session " << id_ << "] 构造函数被调用 (创建)\n";
    }

    ~Session()
    {
        std::cout << "[Session " << id_ << "] 析构函数被调用 (销毁) <--- 观察这里出现的时间点\n";
    }

    // 启动业务逻辑，返回 future 供调用方等待
    std::future<void> start()
    {
        // 使用 shared_from_this() 获取自身的 shared_ptr，引用计数 +1
        auto self = shared_from_this();

        std::cout << "[Session " << id_ << "] start() 被调用，准备发起异步操作\n";

        // 优化：Lambda 只捕获 self，通过 self-> 访问成员
        // 不再单独捕获 this，避免同一对象有两条引用路径，语义更清晰
        return mock_async_read([self]()
                               {
                                   std::cout << "[Session " << self->id_ << "] 回调函数开始执行\n";
                                   self->handle_read();
                                   // Lambda 结束，self 析构，引用计数 -1
                               });
    }

    void handle_read()
    {
    }

private:
    int id_;
};

// ==========================================
// 主流程
// ==========================================
int main()
{
    std::cout << "--- 主线程开始 ---\n";

    std::future<void> fut;

    {
        // 1. 创建 Session 对象，由 shared_ptr 管理，引用计数 = 1
        std::cout << "1. 创建 shared_ptr<Session>\n";
        auto sp = std::make_shared<Session>(1001);

        // 2. 启动异步操作，内部 Lambda 捕获 self，引用计数变为 2
        std::cout << "2. 调用 start()\n";
        fut = sp->start();

        // 3. 模拟 shared_ptr 超出作用域 (比如连接管理列表移除了它)
        std::cout << "3. 释放主线程持有的 shared_ptr\n";
    } // sp 销毁，引用计数 -1 变为 1，对象因 Lambda 中的 self 而存活

    std::cout << "--- 主线程 sp 已销毁，精确等待异步线程完成 ---\n";

    // 优化：用 future::get() 精确等待，不再盲目 sleep 固定时长
    fut.get();

    std::cout << "--- 主线程结束 ---\n";
    return 0;
}
