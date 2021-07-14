#ifndef LIB_SERIAL_H
#define LIB_SERIAL_H


#include <vector>
#include <chrono>

extern "C" {
#include "driver/uart.h"
#include "driver/gpio.h"
}

#include "exception.h"

namespace ansic1218 {

	class SerialException : public Exception {
	public:
		template<typename... Args>
		explicit SerialException(Args &&... args) : Exception(__PRETTY_FUNCTION__, args...) {}
	};

	class Serial {

        uart_port_t uart_num;
        int uart_baud_rate;
        int tx_io_num;
        int rx_io_num;

	public:

		enum class FlushInputBuffer : uint8_t {
			Disable = 0,
			Enable = 1
		} flushInputBuffer;

		Serial();

        ~Serial();

		void open(uart_port_t uart_num, int uart_baud_rate, int tx_io_num, int rx_io_num);

        void set_loop_back();

		virtual void write(const std::vector<uint8_t> &buffer);

		virtual size_t read(std::vector<uint8_t> &buffer, size_t nBytesToRead, std::chrono::nanoseconds timeout);

		virtual size_t read(std::vector<uint8_t> &buffer, size_t nBytesToRead, std::chrono::nanoseconds timeout, FlushInputBuffer flushInputBuffer);

		virtual size_t read(std::vector<uint8_t> &buffer, std::vector<uint8_t> &pattern, size_t nBytesToRead, std::chrono::nanoseconds timeout);

		virtual size_t read(std::vector<uint8_t> &buffer, std::vector<uint8_t> &pattern, size_t nBytesToRead, std::chrono::nanoseconds timeout, FlushInputBuffer flushInputBuffer);

	};
}


#endif //LIB_SERIAL_H
