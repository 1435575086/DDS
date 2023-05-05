#include <iostream>
#include <cstring>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/crc.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <dds/dds.hpp>

// The IDL namespace and topic name
const std::string idl_namespace = "test";
const std::string topic_name = "test_topic";

// The size of the data to be sent
const int data_size = 1024; // 1KB

// The number of iterations for the latency test
const int latency_test_iterations = 10000;

// The type supporting data that we'll send
struct test_data
{
    std::array<char, data_size> data;
    std::string md5;
};

// Print the message loss rate (if applicable)
void print_loss_rate(const int sent, const int received)
{
    if (sent > 0 && received > 0)
    {
        double loss_rate = 100.0 * (1.0 - static_cast<double>(received) / static_cast<double>(sent));
        std::cout << "Loss rate: " << loss_rate << "%" << std::endl;
    }
}

// Calculate the md5 hash of a given buffer
std::string md5(const char* data, const size_t size)
{
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;
    hash.process_bytes(data, size);
    hash.get_digest(digest);
    const char* char_digest = reinterpret_cast<const char*>(digest);
    return std::string(char_digest, char_digest + sizeof(boost::uuids::detail::md5::digest_type));
}

// A function to run the publisher
void run_publisher(const std::string& participant_id, const std::string& topic_id, const bool is_large_data)
{
    // Set up DDS
    dds::domain::DomainParticipant participant(0);

    dds::topic::Topic<test_data> topic(participant, topic_name);

    dds::pub::Publisher publisher(participant);

    dds::pub::DataWriter<test_data> writer(publisher, topic);

    test_data data;
    std::array<char, data_size> buffer;
    std::memset(buffer.data(), 'a', buffer.size());
    data.data = buffer;
    data.md5 = md5(data.data.data(), data.data.size());

    // Send the data
    int sent_count = 0;
    while (true)
    {
        const auto start_time = std::chrono::system_clock::now();
        writer.write(data);
        ++sent_count;
        const auto end_time = std::chrono::system_clock::now();
