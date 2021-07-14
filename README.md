# ESP-IDF ANSI C12.18 Component

ANSI C12.18 component for Espressif ESP32 [ESP-IDF framework](https://github.com/espressif/esp-idf)

ANSI C12.18 is a communication protocol for two-way communication with a metereing device through an ANSI Type 2 Optical port. This component implements ANSI C12.18 funcitonalities over UART port, which can be attached to a optical-serial adapter to interface with the metering device.

## Implemented Services

- Identification Service;
- Read (Total Read) Service;
- Write Service (Total Write);
- Security Service;
- Partial Read (offset/octet-count method);
- Partial Write (offset/octet-count method);

## How to use

### ANSI C12.18 Component

Clone this repository somewhere, e.g.:

```Shell
cd ~/myprojects/esp
git clone https://github.com/tmedicci/esp-idf-ansic1218.git
```

Add path to components in your [CMakeLists.txt](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html):

```CMake
cmake_minimum_required(VERSION 3.5)
set(EXTRA_COMPONENT_DIRS /home/user/myprojects/esp)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my-esp-project)
```

### ANSI C12.18 Examples

Examples directory provides the following usage examples:
- [Meter Identification](examples/meter_identification/README.md)

To build examples, clone this repository somewhere, navigate to the example directory and build it with 'idf.py' e.g.:

```Shell
cd ~/myprojects/esp
git clone https://github.com/tmedicci/esp-idf-ansic1218.git
cd examples/meter_identification
idf.py build
```
