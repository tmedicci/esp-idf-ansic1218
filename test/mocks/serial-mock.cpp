
#include "serial-mock.hpp"

using namespace std;
using namespace chrono;
using namespace mock;

Serial::Serial() : ansic1218::Serial(), request(), response() {}

void Serial::write(const vector<uint8_t> &buffer)
{
    request.insert(request.end(), buffer.begin(), buffer.end());
}

size_t Serial::read(vector<uint8_t> &buffer, size_t nBytesToRead, nanoseconds timeout)
{

    nBytesToRead = nBytesToRead < response.size() ? nBytesToRead : response.size();

    buffer.reserve(buffer.size() + nBytesToRead);

    copy(begin(response), begin(response) + nBytesToRead, back_inserter(buffer));

    response.erase(begin(response), begin(response) + nBytesToRead);

    return nBytesToRead;
}

void Serial::setResponse(const vector<uint8_t> &response)
{
    Serial::response = response;
}

const vector<uint8_t> &Serial::getRequested() const
{
    return request;
}
