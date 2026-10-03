#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <cstring>

#define DEVICE_PATH "/dev/chaos_driver"

#define CHAOS_IOCTL_MAGIC 'C'

struct chaos_config {
    int latency;
    int packet_loss;
    int jitter;
};

#define CHAOS_IOCTL_SET_CONFIG \
    _IOW(CHAOS_IOCTL_MAGIC, 1, struct chaos_config)

#define CHAOS_IOCTL_GET_CONFIG \
    _IOR(CHAOS_IOCTL_MAGIC, 2, struct chaos_config)

int main()
{
    int fd = open(DEVICE_PATH, O_RDWR);

    if (fd < 0) {
        std::cerr << "Failed to open " << DEVICE_PATH
                  << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    chaos_config config{};

    config.latency = 100;
    config.packet_loss = 5;
    config.jitter = 20;

    if (ioctl(fd, CHAOS_IOCTL_SET_CONFIG, &config) < 0) {
        std::cerr << "SET_CONFIG ioctl failed: "
                  << std::strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    std::cout << "Configuration sent to driver:\n";
    std::cout << "Latency: " << config.latency << " ms\n";
    std::cout << "Packet Loss: " << config.packet_loss << "%\n";
    std::cout << "Jitter: " << config.jitter << " ms\n";

    chaos_config received{};

    if (ioctl(fd, CHAOS_IOCTL_GET_CONFIG, &received) < 0) {
        std::cerr << "GET_CONFIG ioctl failed: "
                  << std::strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    std::cout << "\nConfiguration received from driver:\n";
    std::cout << "Latency: " << received.latency << " ms\n";
    std::cout << "Packet Loss: " << received.packet_loss << "%\n";
    std::cout << "Jitter: " << received.jitter << " ms\n";

    close(fd);

    return 0;
}
