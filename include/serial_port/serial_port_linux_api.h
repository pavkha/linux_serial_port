#pragma once

#include "iserial_port.h"

#include <termios.h>
#include <string>

namespace hardware {

class serial_port_linux_api : public iserial_port {
  public:
    explicit serial_port_linux_api(const char* name) : port_name{name} {};
    ~serial_port_linux_api();

    serial_port_linux_api() = default;
    serial_port_linux_api(const serial_port_linux_api&) = delete;
    serial_port_linux_api(serial_port_linux_api&&) = delete;
    serial_port_linux_api& operator=(const serial_port_linux_api&) = delete;
    serial_port_linux_api& operator=(serial_port_linux_api&&) = delete;

    void set_port_name(const char* name) override;
    int open(int baud = 115200,
             const char* params = "8n1",
             const char* flow_control = "none") override;
    int close() override;
    int read(void* buff, std::size_t size) const override;
    int write(const void* buff, std::size_t size) const override;
    int available() const override;

    bool is_open() const override { return fd != -1; }

  private:
    std::string port_name{"not set"};
    int fd{-1};

    termios old_port_settings;
};    // class serial_port_linux_api

}    // namespace hardware
