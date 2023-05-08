#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include <openssl/md5.h>

using namespace std;

const int DATA_SIZE = 1024*1024; //数据大小
const int NUM_TESTS = 10000; //测试次数

//计算MD5值
string md5(const char* data, int size) {
    unsigned char md[MD5_DIGEST_LENGTH];
    MD5((const unsigned char*)data, size, md);

    stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)md[i];
    }
    return ss.str();
}

//发送数据
void send_data() {
    char data[DATA_SIZE];
    memset(data, 'a', DATA_SIZE);
    string md5_value = md5(data, DATA_SIZE);

    //将数据和md5值打包
    vector<char> buffer(DATA_SIZE + md5_value.size());
    memcpy(buffer.data(), data, DATA_SIZE);
    memcpy(buffer.data() + DATA_SIZE, md5_value.c_str(), md5_value.size());

    //发送数据包
    //...
}

//接收数据
void receive_data() {
    char data[DATA_SIZE];
    //接收数据包
    //...

    //解析数据包
    vector<char> buffer(DATA_SIZE + MD5_DIGEST_LENGTH);
    memcpy(buffer.data(), data, DATA_SIZE + MD5_DIGEST_LENGTH);
    char* md5_value = buffer.data() + DATA_SIZE;
    string received_md5_value = md5(data, DATA_SIZE);

    //判断md5值是否一致
    if(strcmp(md5_value, received_md5_value.c_str()) != 0) {
        cout << "Error: MD5 value does not match!" << endl;
    }
}

int main() {
    send_data();
    receive_data();
    return 0;
}
