#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <unistd.h>
#include "dds/dds.hpp"
#include "HelloWorld.hpp"
#include <openssl/md5.h>

 

// 计算文件的MD5值
std::string calcMD5(const std::string& fileName) {
    std::ifstream in(fileName, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening file " << fileName << std::endl;
        exit(1);
    }

 

    MD5_CTX md5;
    MD5_Init(&md5);

 

    char buf[4096];
    while (in.read(buf, sizeof(buf))) {
        MD5_Update(&md5, buf, sizeof(buf));
    }
    MD5_Update(&md5, buf, in.gcount());

 

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &md5);

 

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < sizeof(digest); i++) {
        ss << std::setw(2) << (int)digest[i];
    }

 

    return ss.str();
}

 

int main(int argc, char *argv[]) {
    dds::domain::DomainParticipant participant(org::eclipse::cyclonedds::domain::default_id());

 

    // 为订阅者创建 Topic
    dds::topic::Topic<HelloWorld::Msg> topic(participant, "Example HelloWorld");

 

    // 为订阅者创建 Subscriber
    dds::sub::Subscriber subscriber(participant);

 

    // 创建 DataReader
    dds::sub::DataReader<HelloWorld::Msg> reader(subscriber, topic);

 

    while (true) {
        // 从 DataReader 中读取消息
        auto samples = reader.take();
        for (const auto& sample : samples) {
            if (sample.info().valid()) {
                std::cout << "Received a message:" << std::endl;
                std::cout << "  Message ID: " << sample.data().id() << std::endl;
                std::cout << "  Message size: " << sample.data().data().size() << " bytes" << std::endl;
                std::string calc_md5 = calcMD5(sample.data().data());
                std::cout << "  Message MD5: " << calc_md5 << std::endl;
                std::cout << "  Received MD5: " << sample.data().md5() << std::endl;
                if (calc_md5 != sample.data().md5()) {
                    std::cerr << "Error: MD5 check failed!" << std::endl;
                }
            }
        }

 

        // 暂停 1 秒
        usleep(1000000);
    }

 

    return 0;
}
