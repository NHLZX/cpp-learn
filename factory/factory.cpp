#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <memory>
class ExecutorBase
{

public:
    virtual ~ExecutorBase() = default;
    virtual void doProcess() = 0;
};
class Planner
{
public:
    virtual ~Planner() = default;
    virtual void doPlan() = 0;
};

class dwaPlanner : public Planner
{
public:
    void doPlan() override
    {
        std::cout << "进行dwa规划" << std::endl;
    }
};

class AstarPlanner : public Planner
{
public:
    void doPlan() override
    {
        std::cout << "进行A*规划" << std::endl;
    }
};

class PlannerFactory
{
public:
    using Creator = std::function<Planner *()>;
    static PlannerFactory &Instance()
    {
        static PlannerFactory fac;
        return fac;
    }
    void Register(const std::string name, Creator creator)
    {
        map[name] = creator;
    }
    Planner *Create(const std::string &name)
    {
        if (map.find(name) != map.end())
        {
            return map[name]();
        }
        return nullptr;
    }
    std::map<std::string, Creator> map;
};

class RegisterClass
{
public:
    RegisterClass(const std::string &name, PlannerFactory::Creator creator)
    {
        PlannerFactory::Instance().Register(name, creator);
    }
};
std::string s1 = "dwa";

static RegisterClass res1("dwa", []()
                          { return new dwaPlanner(); });

class ExecutorFactory
{
public:
    using Creator = std::function<ExecutorBase *()>;
    static ExecutorFactory &Instance()
    {
        static ExecutorFactory factory;
        return factory;
    }

    // 所有的先注册，然后放入到map中

    void Register(const std::string name, Creator creator)
    {
        map[name] = creator;
    }

    // 使用的是否，返回基类指针，指向派生类
    ExecutorBase *Create(const std::string &name)
    {
        if (map.find(name) != map.end())
        {
            return map[name](); // 执行生产逻辑
        }
        return nullptr;
    }

private:
    std::map<std::string, Creator> map;
    ExecutorFactory() = default; // 私有化构造函数
};

class ExecutorHmiPerception : public ExecutorBase
{
public:
    void doProcess() override
    {
        std::cout << "Rendering 3D World Model..." << std::endl;
    }
    // 工厂方法：静态函数，不依赖实例即可运行
    static ExecutorBase *CreateInstance() { return new ExecutorHmiPerception(); }
};

// 执行器 B：交互处理
class ExecutorInteract : public ExecutorBase
{
public:
    void doProcess() override
    {
        std::cout << "Triggering Interaction Events..." << std::endl;
    }
    static ExecutorBase *CreateInstance() { return new ExecutorInteract(); }
};

// 注册代理类
class Registerer
{
public:
    Registerer(const std::string &name, ExecutorFactory::Creator creator)
    {
        ExecutorFactory::Instance().Register(name, creator);
    }
};

// 关键点：在全局空间定义变量，程序启动时会自动执行构造函数，完成注册
static Registerer g_reg_perc("executorHmiPerception", &ExecutorHmiPerception::CreateInstance);
static Registerer g_reg_intr("executorInteract", &ExecutorInteract::CreateInstance);

int main()
{
    // 模拟从配置文件（.dag）读取到的字符串
    std::string config_name = "executorHmiPerception";

    ExecutorBase *myExecutor = ExecutorFactory::Instance().Create(config_name);

    if (myExecutor)
    {
        myExecutor->doProcess(); // 执行业务
        delete myExecutor;       // 销毁
    }

    return 0;
}
