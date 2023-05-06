#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std;
using namespace std::chrono;

int main()
{
    int dataSize = 1024; // 默认数据大小为1KB
    int loopCount = 10000; // 默认循环次数为10000
    double totalDelay = 0; // 初始化总时延为0

    cout << "请输入数据大小（单位：Byte）：" << endl;
    cin >> dataSize;

    cout << "请输入循环次数：" << endl;
    cin >> loopCount;

    // 定义发送和接收时间点
    time_point<system_clock> sendTime, recvTime;

    for (int i = 0; i < loopCount; i++)
    {
        // A 发送数据
        sendTime = system_clock::now();
        // TODO: 发送数据

        // B 接收数据并回复
        // TODO: 接收数据并回复
        recvTime = system_clock::now();

        // A 收到回复并计算时延
        duration<double, micro> delay = duration_cast<duration<double, micro>>(recvTime - sendTime);
        totalDelay += delay.count();

        cout << "第" << i + 1 << "次发送数据的时延为：" << delay.count() << " us" << endl;
    }

    // 计算平均时延并输出
    double averageDelay = totalDelay / loopCount;
    cout << "端到端平均时延为：" << averageDelay << " us" << endl;

    return 0;
}
