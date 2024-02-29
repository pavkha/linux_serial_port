#pragma once

#include <cstdint>

namespace hardware {

class iserial_port {
  public:
    virtual ~iserial_port() = default;

    virtual void set_port_name(const char* name) = 0;
    virtual int open(int baud, const char* params, const char* flow_control) = 0;
    virtual int close() = 0;
    virtual int read(void* buff, std::size_t size) const = 0;
    virtual int write(const void* buff, std::size_t size) const = 0;
    virtual int available() const = 0;
    virtual bool is_open() const = 0;
};    // class iserial_port

}    // namespace hardware
