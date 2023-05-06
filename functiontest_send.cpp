#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <cstring>
#include <cstdlib>
#include <ctime>

 

#include <dds/dds.hpp>
#include "msg.hpp"

 

// Constants
const int ONE_MB = 1024 * 1024;
const int MAX_BUFFER_SIZE = ONE_MB;
const int MAX_SAMPLES = 1;
const char *DDS_TOPIC = "DDS_Test_Topic";

 

// Function to generate random data of a given size
void generate_random_data(unsigned char *buffer, int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (int i = 0; i < size; ++i) {
        buffer[i] = dis(gen);
    }
}

 

// Function to calculate the MD5 hash of a buffer
void md5(unsigned char *buffer, int size, unsigned char *hash) {
    // TODO: Implement MD5 hash calculation
    // You can use external libraries or implement it yourself
}

 

int main(int argc, char *argv[]) {
    // Parse command line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <data_size>" << std::endl;
        return 1;
    }
    int data_size = atoi(argv[1]);
    if (data_size <= 0 || data_size > MAX_BUFFER_SIZE) {
        std::cerr << "Data size must be between 1 and " << MAX_BUFFER_SIZE << " bytes" << std::endl;
        return 1;
    }

 

    // Generate random data and calculate MD5 hash
    unsigned char data[data_size];
    generate_random_data(data, data_size);
    unsigned char md5_hash[16];
    md5(data, data_size, md5_hash);

 

    // Initialize DDS
    dds::domain::DomainParticipant participant(0);
    dds::topic::Topic<DDS_Test_Msg> topic(participant, DDS_TOPIC);
    dds::pub::DataWriter<DDS_Test_Msg> writer(dds::pub::Publisher(participant), topic);

 

    // Prepare DDS message
    DDS_Test_Msg msg;
    msg.buffer().resize(data_size);
    std::memcpy(msg.buffer().data(), data, data_size);
    msg.md5_hash().resize(sizeof(md5_hash));
    std::memcpy(msg.md5_hash().data(), md5_hash, sizeof(md5_hash));

 

    // Publish DDS message
    auto start_time = std::chrono::steady_clock::now();
    writer.write(msg);
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::cout << "Sent " << data_size << " bytes in " << elapsed_time << " microseconds ("
<< (double)data_size / (double)elapsed_time << " MB/s)" << std::endl;

 

    return 0;
}
