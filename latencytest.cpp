#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

 

using namespace std;

 

int main(int argc, char **argv) {
    if (argc < 4) {
        cerr << "Usage: ./latency_test <ip> <port> <data_size>" << endl;
        return -1;
    }

 

    // 创建发送和接收socket
    int send_fd = socket(AF_INET, SOCK_DGRAM, 0);
    int recv_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_fd < 0 || recv_fd < 0) {
        perror("create socket error");
        return -1;
    }

 

    // 绑定接收socket
    struct sockaddr_in recv_addr;
    bzero(&recv_addr, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(atoi(argv[2]));
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(recv_fd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        perror("bind socket error");
        return -1;
    }

 

    // 设置发送socket参数，以提高发送性能
    int send_buf_size = atoi(argv[3]);
    if (setsockopt(send_fd, SOL_SOCKET, SO_SNDBUF, &send_buf_size, sizeof(send_buf_size)) < 0) {
        perror("set send buffer size error");
        return -1;
    }

 

    // 构造发送数据
    char send_buf[send_buf_size];
    bzero(send_buf, sizeof(send_buf));
    strncpy(send_buf, "hello", sizeof("hello"));

 

    // 发送和接收数据，计算时延
    int total_loop = 10000;  // 循环次数
    int count = 0;
    double total_latency = 0.0;
    while (count < total_loop) {
        // 发送数据
        auto send_time = chrono::high_resolution_clock::now();
        if (sendto(send_fd, send_buf, send_buf_size, 0, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
            perror("send data error");
            return -1;
        }

 

        // 接收数据
        char recv_buf[send_buf_size];
        bzero(recv_buf, sizeof(recv_buf));
        socklen_t recv_addr_len = sizeof(recv_addr);
        if (recvfrom(recv_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&recv_addr, &recv_addr_len) < 0) {
            perror("recv data error");
            return -1;
        }

 

        // 计算时延
        auto recv_time = chrono::high_resolution_clock::now();
        double latency = chrono::duration_cast<chrono::microseconds>(recv_time - send_time).count() / 2.0;
        total_latency += latency;
        count++;
    }

 

    // 计算平均时延
    double avg_latency = total_latency / count;
    cout << "Average latency: " << avg_latency << " us" << endl;

 

    // 关闭socket
    close(send_fd);
    close(recv_fd);

 

    return 0;
}
