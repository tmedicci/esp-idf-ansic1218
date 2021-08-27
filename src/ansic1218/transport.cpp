// Copyright 2021 Time Energy Industria e Comercio Ltda
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <algorithm>
#include <cassert>
#include <iomanip>
#include <endian.h>
#include <machine/endian.h>
#include "transport.hpp"

using namespace std;
using namespace chrono;
using namespace ansic1218;
using namespace service;
using namespace utils;

static const char *TAG = "ansic1218::transport";

struct Transport::Packet {
    enum
    {
        START_OF_PACKET = 0xee,
        IDENTITY = 0x05
    };

    uint8_t stp;
    uint8_t identity;
    uint8_t ctrl;
    uint8_t seq_nbr;
    uint16_t length;

} __attribute__((__packed__));

Transport::Transport(shared_ptr<Serial> serial) : serial(move(serial)) {}

bool Transport::request(service::Service &&service)
{
    return request(service);
}

bool Transport::request(service::Service &service)
{
    Packet packet{.stp = Packet::START_OF_PACKET, .identity = Packet::IDENTITY, .ctrl = 0, .seq_nbr = 0, .length = 0};

    const uint8_t MULTI_PACKET = 0b10000000;
    const uint8_t FIRST_PACKET = 0b01000000;

    send({ACK});

    static_assert(sizeof(packet) == 6, "");

    auto *ptr = reinterpret_cast<uint8_t *>(&packet);

    vector<uint8_t> sent{ptr, &ptr[sizeof(packet)]};

    if (!service.request(sent)) {
        ESP_LOGW(TAG, "Could not properly build request frame");
        return false;
    }

    auto *p_packet = reinterpret_cast<Packet *>(sent.data());
    p_packet->length = htobe16(sent.size() - sizeof(Packet));

    CRC::calculate(sent.cbegin(), sent.cend(), sent);
    send(sent);

    vector<uint8_t> content;
    uint8_t max_request = 0;
    int seq_nbr = 0;

    vector<uint8_t> received;

    bool ret = wait(received, {ACK});
    while (!ret && max_request < 3) {
        max_request++;
        ESP_LOGD(TAG, "Sending packet again. Requested: %d times", int(max_request));
        flush();
        send(sent);
        received.clear();
        ret = wait(received, {ACK});
    }

    if (!ret)
        return nack(service, sent, received, "ACK not received.");

    do {
        received.clear();

        if (!receive(received, sizeof(packet))) {
            return nack(service, sent, received, "failed to receive header.");
        }

        ESP_LOGD(TAG, "received:");
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, received.data(), received.size(), ESP_LOG_DEBUG);
        p_packet = reinterpret_cast<Packet *>(received.data() + received.size() - sizeof(packet));
        if (!validate(p_packet)) {
            return nack(service, sent, received, "received an invalid header");
        }

        seq_nbr = int(p_packet->seq_nbr);

        if (p_packet->ctrl & MULTI_PACKET) {
            ESP_LOGD(TAG, "Received a multi-packet response.");
            ESP_LOGD(TAG, "Sequence number of the packet is: %d", seq_nbr);
            if (p_packet->ctrl & FIRST_PACKET)
                ESP_LOGD(TAG, "First frame of multi-packet response.");
        }

        if (!receive(received, be16toh(p_packet->length))) {
            return nack(service, sent, received, "failed to receive response.");
        }

        auto crc = CRC::calculate(received.cbegin(), received.cend(), {});

        if (!wait(received, crc)) {
            return nack(service, sent, received, "failed to receive CRC");
        }

        send({ACK});

        copy(received.cbegin() + sizeof(Packet), received.cend() - crc.size(), back_inserter(content));

    } while (seq_nbr > 0);

    return service.response(content.cbegin(), content.cend());
}

void Transport::send(const vector<uint8_t> &data)
{
    serial->write(data);
    ESP_LOGV(TAG, "send(): %s .", bufToStr(data.cbegin(), data.cend()).c_str());
}

bool Transport::wait(std::vector<uint8_t> &buffer, const vector<uint8_t> &data)
{
    auto nBytesRead = receive(buffer, data.size());

    if (nBytesRead <= 0)
        return false;

    if (!equal(data.cbegin(), data.cend(), buffer.cend() - nBytesRead)) {
        ESP_LOGW(TAG, "Wait()  expected: %s, received: %s .", bufToStr(data.cbegin(), data.cend()).c_str(),
                 bufToStr(buffer.cend() - nBytesRead, buffer.cend()).c_str());
        return false;
    }

    return true;
}

int Transport::receive(vector<uint8_t> &buffer, size_t size)
{
    auto nBytesRead = serial->read(buffer, size, seconds(2));

    if (nBytesRead != size) {
        ESP_LOGD(TAG, "Receive() received less bytes than expected. expected: %d, received: %d", size, nBytesRead);
        return false;
    }

    ESP_LOGD(TAG, "received(): %s .", bufToStr(buffer.cbegin(), buffer.cend()).c_str());

    return nBytesRead;
}

void Transport::flush()
{
    for (vector<uint8_t> buffer; 0 < serial->read(buffer, 100, milliseconds(100));) {
        ESP_LOGD(TAG, "Flushed:  %s", bufToStr(buffer.cbegin(), buffer.cend()).c_str());
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, buffer.data(), buffer.size(), ESP_LOG_DEBUG);
        buffer.clear();
    }
}

bool Transport::nack(const vector<uint8_t> &sent, const vector<uint8_t> &received)
{
    ESP_LOGW(TAG, "Sent:     %s", bufToStr(sent.cbegin(), sent.cend()).c_str());
    ESP_LOGW(TAG, "Received: %s", bufToStr(received.cbegin(), received.cend()).c_str());

    flush();

    send({ACK});
    return false;
}

bool Transport::validate(Packet *packet)
{
    return packet->stp == Packet::START_OF_PACKET;
}

std::string Transport::bufToStr(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    ostringstream ss;
    ss << "{ ";
    for_each(first, last, [&ss](uint8_t value) { ss << setfill('0') << setw(2) << hex << int(value) << " "; });
    ss << "}";
    return ss.str();
}
