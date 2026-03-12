#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::placeholders;

using CallbackType = function<void(string)>;

class Downloader
{
public:
    void setOnCompleteCallback(CallbackType cb)
    {
        m_callback = cb;
    }
    void download(string url)
    {
        cout << "Downloader" << url << endl;
        this_thread::sleep_for(chrono::seconds(1));

        string result = "content of" + url;

        if (m_callback)
        {
            cout << "Finished Calling back" << endl;
            m_callback(result); // 触发回调
        }
    }

private:
    CallbackType m_callback;
};

class DisPlay
{
public:
    void showText(string content)
    {
        cout << "showing:" << content << endl;
    }
};

int main()
{
    Downloader dl;
    DisPlay display;

    auto cb = bind(&DisPlay::showText, &display, _1);

    dl.setOnCompleteCallback(cb);
    dl.download("http://google.com");
    dl.setOnCompleteCallback([&display](string s)
                             { display.showText(s); });
    dl.download("http://github.com");
    return 0;
}