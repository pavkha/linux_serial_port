#include "serial_port_linux_api.h"

#include <gtest/gtest.h>
#include <array>
#include <chrono>
#include <thread>

// For test used usb/serial converter U-400 with four ports
const char* port_name = "/dev/ttyUSB3";

TEST(serial_port_linux_api_test, open) {
    hardware::serial_port_linux_api serial_port{port_name};

    EXPECT_NE(serial_port.open(921600, "8n1"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(921600, "8n0"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(921601, "8n1"), -1);
    serial_port.close();

    EXPECT_NE(serial_port.open(115200, "8n1"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(115200, "8n0"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(115201, "8n1"), -1);
    serial_port.close();

    EXPECT_NE(serial_port.open(57600, "8n1"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(57600, "8n0"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(57601, "8n1"), -1);
    serial_port.close();

    EXPECT_NE(serial_port.open(38400, "8n1"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(38400, "8n0"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(38401, "8n1"), -1);
    serial_port.close();

    EXPECT_NE(serial_port.open(19200, "8n1"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(19200, "8n0"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(19201, "8n1"), -1);
    serial_port.close();

    EXPECT_NE(serial_port.open(9600, "8n1"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(9600, "8n0"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(9601, "8n1"), -1);
    serial_port.close();

    EXPECT_NE(serial_port.open(4800, "8n1"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(4800, "8n0"), -1);
    serial_port.close();

    EXPECT_EQ(serial_port.open(4801, "8n1"), -1);
}

TEST(serial_port_linux_api_test, set_port_name) {
    hardware::serial_port_linux_api serial_port{};

    serial_port.set_port_name(port_name);
    EXPECT_NE(serial_port.open(115200, "8n1"), -1);
    serial_port.close();

    serial_port.set_port_name("/dev/ttyFAKE");
    EXPECT_EQ(serial_port.open(115200, "8n1"), -1);
}

// For tests write/available/read connect pins TX(3) & RX(2) in DB9-M
TEST(serial_port_linux_api_test, available) {
    hardware::serial_port_linux_api serial_port{port_name};
    std::array<char, 'Z' - 'A' + 1> tx_buff;

    for (auto c = 'A'; auto& e : tx_buff) {
        e = c++;
    }

    ASSERT_NE(serial_port.open(115200, "8n1"), -1);
    ASSERT_EQ(serial_port.write(tx_buff.data(), tx_buff.size()), tx_buff.size());

    std::chrono::milliseconds rx_timeout{100};
    std::this_thread::sleep_for(rx_timeout);

    EXPECT_EQ(serial_port.available(), tx_buff.size());
}

TEST(serial_port_linux_api_test, write_read) {
    hardware::serial_port_linux_api serial_port{port_name};
    std::array<std::uint8_t, 'Z' - 'A' + 1> tx_buff, rx_buff;
    std::chrono::milliseconds rx_timeout{100};

    for (std::uint8_t c = 'A'; auto& e : tx_buff) {
        e = c++;
    }

    EXPECT_NE(serial_port.open(921600, "8n1"), -1);
    EXPECT_EQ(serial_port.write(tx_buff.data(), tx_buff.size()), tx_buff.size());

    std::this_thread::sleep_for(rx_timeout);

    EXPECT_EQ(serial_port.available(), tx_buff.size());
    EXPECT_NE(serial_port.read(rx_buff.data(), rx_buff.size()), -1);

    EXPECT_TRUE(rx_buff == tx_buff);
    serial_port.close();

    EXPECT_NE(serial_port.open(115200, "8n1"), -1);
    EXPECT_EQ(serial_port.write(tx_buff.data(), tx_buff.size()), tx_buff.size());

    std::this_thread::sleep_for(rx_timeout);

    EXPECT_EQ(serial_port.available(), tx_buff.size());
    EXPECT_NE(serial_port.read(rx_buff.data(), rx_buff.size()), -1);

    EXPECT_TRUE(rx_buff == tx_buff);
    serial_port.close();

    EXPECT_NE(serial_port.open(57600, "8n1"), -1);
    EXPECT_EQ(serial_port.write(tx_buff.data(), tx_buff.size()), tx_buff.size());

    std::this_thread::sleep_for(rx_timeout);

    EXPECT_EQ(serial_port.available(), tx_buff.size());
    EXPECT_NE(serial_port.read(rx_buff.data(), rx_buff.size()), -1);

    EXPECT_TRUE(rx_buff == tx_buff);
    serial_port.close();

    EXPECT_NE(serial_port.open(19200, "8n1"), -1);
    EXPECT_EQ(serial_port.write(tx_buff.data(), tx_buff.size()), tx_buff.size());

    std::this_thread::sleep_for(rx_timeout);

    EXPECT_EQ(serial_port.available(), tx_buff.size());
    EXPECT_NE(serial_port.read(rx_buff.data(), rx_buff.size()), -1);

    EXPECT_TRUE(rx_buff == tx_buff);
    serial_port.close();

    EXPECT_NE(serial_port.open(9600, "8n1"), -1);
    EXPECT_EQ(serial_port.write(tx_buff.data(), tx_buff.size()), tx_buff.size());

    std::this_thread::sleep_for(rx_timeout);

    EXPECT_EQ(serial_port.available(), tx_buff.size());
    EXPECT_NE(serial_port.read(rx_buff.data(), rx_buff.size()), -1);

    EXPECT_TRUE(rx_buff == tx_buff);
}
