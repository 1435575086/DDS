#include <iostream>
#include <cstring>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int DATA_SIZE = 1024*1024; //数据大小

//发送数据
void send_data() {
    char data[DATA_SIZE];
    memset(data, 'a', DATA_SIZE);

    //发送数据包
    //...
}

//接收数据
void receive_data() {
    char data[DATA_SIZE];
    int num_received = 0;
    double prev_time = 0;

    while(true) {
        //接收数据包
        //...

        num_received++;
        double current_time = duration_cast<duration<double>>(high_resolution_clock::now().time_since_epoch()).count();

        //计算吞吐量
        if(current_time - prev_time >= 1) {
            double throughput = num_received * DATA_SIZE * 8 / (current_time - prev_time) / 1000000;
            cout << "Throughput: " << throughput << " Mbps" << endl;

            prev_time = current_time;
            num_received = 0;
        }
    }
}

int main() {
    send_data();
    receive_data();
    return 0;
}
