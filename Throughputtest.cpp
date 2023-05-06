#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <chrono>
#include <thread>
#include <dds/dds.hpp>
#include "example.hpp"

 

using namespace dds::core;
using namespace dds::domain;
using namespace dds::topic;
using namespace dds::pub;
using namespace dds::sub;
using namespace dds::sub::qos;
using namespace dds::core::policy;
using namespace std::chrono_literals;

 

class TestSubscriber {
public:
    TestSubscriber() : participant_(default_domain()),
                       subscriber_(participant_, SubscriberQos(),
                                  nullptr, STATUS_MASK_NONE),
                       md5_reader_(subscriber_, md5_topic_) {
        md5_reader_.listener(this, dds::core::status::StatusMask::data_available());
    }

 

    ~TestSubscriber() {
    }

 

    void receive() {
        int count = 0;
        int total_bytes = 0;
        int total_lost = 0;
        int total_packets = 0;
        bool first_packet = true;
        bool lost_packet = false;

 

        while (true) {
            dds::sub::LoanedSamples<Md5> samples = md5_reader_.take();
            for (const auto& sample : samples) {
                if (sample.info().valid()) {
                    if (first_packet) {
                        first_packet = false;
                        lost_packet = false;
                    } else {
                        int lost = sample.data().seq_num() - prev_seq_num_ - 1;
                        if (lost > 0) {
                            total_lost += lost;
                            lost_packet = true;
                        } else {
                            lost_packet = false;
                        }
                    }
                    prev_seq_num_ = sample.data().seq_num();
                    int size = sample.data().data().size();
                    total_bytes += size;
                    total_packets++;
                }
            }
            std::this_thread::sleep_for(1s);
            double rate = total_bytes * 8 / (1024 * 1024.0);
            std::cout << "Received " << total_packets << " packets, "
<< total_bytes << " bytes, " << rate << " Mbps";
            if (total_packets > 0) {
                double loss_rate = total_lost * 1.0 / total_packets * 100.0;
                std::cout << ", " << loss_rate << "% packets lost";
            }
            std::cout << std::endl;
        }
    }

 

private:
    DomainParticipant participant_;
    Subscriber subscriber_;
    Topic<Md5> md5_topic_{participant_, "Example md5"};
    DataReader<Md5> md5_reader_;
    int prev_seq_num_ = -1;
};

 

int main(int argc, char* argv[]) {
    TestSubscriber subscriber;
    subscriber.receive();
    return 0;
}
