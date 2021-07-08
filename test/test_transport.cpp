
#include "unity.h"

#include "ansic1218/transport.h"
#include "driver/uart.h"

using namespace std;
using namespace ansic1218;
using namespace ansic1218::service;

extern "C" {
#include "Mockuart.h"
}

class MockedService : public Service {

	vector<uint8_t> _request;
	vector<uint8_t> _data;

public:

	MockedService() : Service(__PRETTY_FUNCTION__), _request(), _data() {}

	void setRequestContent(const vector<uint8_t> &request) {
		_request = request;
	}

	bool request(std::vector<uint8_t> &buffer) override {
		copy(_request.cbegin(), _request.cend(), back_inserter(buffer));
		return true;
	}

	bool response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override {
		_data.reserve(distance(first, last));
		_data.clear();

		copy(first, last, back_inserter(_data));

		return true;
	}

	vector<uint8_t> &data() {
		return _data;
	}
};

int fill_uart_rx_buffer_ref(uart_port_t uart_num, void* buf, uint32_t length, TickType_t ticks_to_wait, int cmock_num_calls)
{
    vector<uint8_t> ref{1, 2};

    memcpy(buf, ref.data(), length);

    return (int)length;
}

/*This test shows the mock actually works while mocking the uart behavior*/
TEST_CASE("Should check whether the uart's mock is actually functional", "[MockedUart]")
{
    vector<uint8_t> ref{1, 2};

    vector<uint8_t> buffer;

    uint32_t size = 2;

    buffer.resize(buffer.size() + size);

    auto ptr = buffer.data() + buffer.size() - size;

    uart_read_bytes_AddCallback(fill_uart_rx_buffer_ref);

    uart_read_bytes_ExpectAndReturn(2, ptr, size, 2000 / portTICK_RATE_MS, size);

    int nBytesRead = uart_read_bytes(2, ptr, size, 2000 / portTICK_RATE_MS);

    TEST_ASSERT(nBytesRead == size);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ref.data(), buffer.data(), size);
}

vector<uint8_t> set_service_request(MockedService &service)
{
    vector<uint8_t> service_request{1, 2, 3, 4, 5, 6};
    service.setRequestContent(service_request);

    return service_request;
}

vector<uint8_t> set_expected_request(MockedService &service)
{
    auto service_request = set_service_request(service);

    vector<uint8_t> expected_request{0x06, 0xee, 5, 0, 0, 0, uint8_t(service_request.size())};
    copy(service_request.begin(), service_request.end(), back_inserter(expected_request));
    CRC::calculate(expected_request.begin() + 1, expected_request.end(), expected_request);
    expected_request.push_back(0x06);

    return expected_request;
}

vector<uint8_t> set_service_content()
{
    vector<uint8_t> content{7, 8, 9, 10, 11, 12, 13, 14};

    return content;
}

vector<uint8_t> get_expected_response() {

    auto content = set_service_content();
    vector<uint8_t> response{0x06, 0xee, 0, 0, 0, 0, uint8_t(content.size())};

    copy(content.begin(), content.end(), back_inserter(response));
    CRC::calculate(response.begin() + 1, response.end(), response);

    return response;
}

int set_uart_tx_buffer_expected_request(uart_port_t uart_num, const void* src, size_t size, int cmock_num_calls)
{
    MockedService service;

    auto expected_request = set_expected_request(service);

    vector<uint8_t> actual_request((uint8_t*)src, (uint8_t*)src + size);

    if (actual_request == expected_request)
    {
        return (int)size;
    } else
    {
        return 0;
    }
}


int fill_uart_rx_buffer_with_expected_response(uart_port_t uart_num, void* buf, uint32_t length, TickType_t ticks_to_wait, int NumCalls)
{
    auto ref = get_expected_response();

    memcpy(buf, ref.data(), length);

    return (int)length;
}

/*This fails because the uart_write/read_bytes funcion is called
multiple times inside the component. It is not possible to use the
mocked uart in such a code. Please, take this only as reference*/
TEST_CASE("Should request and respond properly", "[transport][MockedUart]")
{
    MockedService service;

    uart_driver_install_ExpectAnyArgsAndReturn(ESP_OK);
    uart_param_config_ExpectAnyArgsAndReturn(ESP_OK);
    uart_set_pin_ExpectAnyArgsAndReturn(ESP_OK);

    Transport transport(UART_NUM_2, 9600, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    auto expected_request = set_expected_request(service);

    uart_write_bytes_AddCallback(set_uart_tx_buffer_expected_request);

    uart_write_bytes_ExpectAndReturn(UART_NUM_2, expected_request.data(), expected_request.size(), 0);

    auto expected_response = get_expected_response();

    vector<uint8_t> uart_response;

    auto size = expected_response.size();

    uart_response.resize(uart_response.size() + size);

    auto ptr = uart_response.data() + uart_response.size() - size;

    uart_read_bytes_AddCallback(fill_uart_rx_buffer_with_expected_response);

    uart_read_bytes_ExpectAndReturn(2, ptr, size, 2000 / portTICK_RATE_MS, size);

    vector<uint8_t> service_content_expected = set_service_content();
    vector<uint8_t> service_content_actual = service.data();
    TEST_ASSERT_TRUE(transport.request(service));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(service_content_expected.data(), service_content_actual.data(), service_content_expected.size());
}
