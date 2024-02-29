#include "serial_port_linux_api.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstring>

namespace hardware {

serial_port_linux_api::~serial_port_linux_api() {
    if (fd != -1) {
        tcsetattr(fd, TCSANOW, &old_port_settings);
        ioctl(fd, TIOCNXCL);
        ioctl(fd, TCFLSH, 0);
        ::close(fd);
    }
}

void serial_port_linux_api::set_port_name(const char* name) {
    port_name = name;
}

int serial_port_linux_api::open(int baud, const char* params, const char* flow_control) {
    if (is_open()) {
        return -1;
    }

    auto error_open = [this]() -> int {
        close();

        return -1;
    };

    if ((fd = ::open(port_name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {
        return error_open();
    }

    if (ioctl(fd, TIOCEXCL) < 0) {
        return error_open();
    }

    if (tcgetattr(fd, &old_port_settings) < 0) {
        return error_open();
    }

    termios port_settings{old_port_settings};

    cfmakeraw(&port_settings);

    if (std::string{params} == "8n1") {
        port_settings.c_cflag |= CS8;
    } else {
        return error_open();
    }

    if (std::string{flow_control} == "hw") {
        port_settings.c_cflag |= CRTSCTS;
    } else if (std::string{flow_control} == "sw") {
        port_settings.c_iflag |= (IXON | IXOFF | IXANY);
    }

    port_settings.c_cflag |= CREAD;
    port_settings.c_cflag |= CLOCAL;

    port_settings.c_cc[VTIME] = 0;
    port_settings.c_cc[VMIN] = 1;

    speed_t speed{};

    switch (baud) {
        case 921600:
            speed = B921600;
            break;
        case 115200:
            speed = B115200;
            break;
        case 57600:
            speed = B57600;
            break;
        case 38400:
            speed = B38400;
            break;
        case 19200:
            speed = B19200;
            break;
        case 9600:
            speed = B9600;
            break;
        case 4800:
            speed = B4800;
            break;
        default:
            return error_open();
    }

    cfsetospeed(&port_settings, speed);
    cfsetispeed(&port_settings, speed);

    if (tcsetattr(fd, TCSANOW, &port_settings) < 0) {
        return error_open();
    }

    ioctl(fd, TCFLSH, 0);

    return 0;
}

int serial_port_linux_api::available() const {
    if (!is_open()) {
        return -1;
    }

    int bytes{};

    if (int ret = ioctl(fd, FIONREAD, &bytes) < 0) {
        return ret;
    }

    return bytes;
}

int serial_port_linux_api::read(void* buff, std::size_t size) const {
    if (!is_open()) {
        return -1;
    }

    return static_cast<int>(::read(fd, buff, size));
}

int serial_port_linux_api::write(const void* buff, std::size_t size) const {
    if (!is_open()) {
        return -1;
    }

    return static_cast<int>(::write(fd, buff, size));
}

int serial_port_linux_api::close() {
    if (!is_open()) {
        return -1;
    }

    tcsetattr(fd, TCSANOW, &old_port_settings);
    ioctl(fd, TIOCNXCL);
    ioctl(fd, TCFLSH, 0);

    int ret{-1};
    ret = ::close(fd);

    fd = -1;

    return ret;
}

}    // namespace hardware
