#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>

#define DEVICE_PATH "/dev/chaos_driver"
#define NETWORK_INTERFACE "enp0s3"

#define CHAOS_IOCTL_MAGIC 'C'

struct chaos_config
{
    int latency;
    int packet_loss;
    int jitter;
};

#define CHAOS_IOCTL_SET_CONFIG \
    _IOW(CHAOS_IOCTL_MAGIC, 1, struct chaos_config)

#define CHAOS_IOCTL_GET_CONFIG \
    _IOR(CHAOS_IOCTL_MAGIC, 2, struct chaos_config)


bool apply_network_config(const chaos_config& config)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        std::cerr << "Error: fork() failed.\n";
        return false;
    }

    if (pid == 0)
    {
        std::string delay =
            std::to_string(config.latency) + "ms";

        std::string jitter =
            std::to_string(config.jitter) + "ms";

        std::string loss =
            std::to_string(config.packet_loss) + "%";

        execl(
            "/sbin/tc",
            "tc",
            "qdisc",
            "replace",
            "dev",
            NETWORK_INTERFACE,
            "root",
            "netem",
            "delay",
            delay.c_str(),
            jitter.c_str(),
            "loss",
            loss.c_str(),
            (char*)nullptr
        );

        std::cerr << "Error: Failed to execute tc.\n";
        _exit(1);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    return WIFEXITED(status) &&
           WEXITSTATUS(status) == 0;
}


chaos_config get_profile(const std::string& profile)
{
    chaos_config config{};

    if (profile == "3G")
    {
        config.latency = 100;
        config.packet_loss = 2;
        config.jitter = 30;
    }
    else if (profile == "4G")
    {
        config.latency = 50;
        config.packet_loss = 1;
        config.jitter = 15;
    }
    else if (profile == "Wi-Fi")
    {
        config.latency = 20;
        config.packet_loss = 1;
        config.jitter = 5;
    }
    else if (profile == "Satellite")
    {
        config.latency = 600;
        config.packet_loss = 2;
        config.jitter = 50;
    }

    return config;
}


int main()
{
    std::cout << "====================================\n";
    std::cout << " Network Latency & Packet Loss\n";
    std::cout << " Chaos Emulator\n";
    std::cout << "====================================\n\n";

    std::cout << "Select Network Profile:\n";
    std::cout << "1. 3G\n";
    std::cout << "2. 4G\n";
    std::cout << "3. Wi-Fi\n";
    std::cout << "4. Satellite\n";
    std::cout << "\nEnter choice: ";

    int choice;
    std::cin >> choice;

    std::string profile;

    switch (choice)
    {
        case 1:
            profile = "3G";
            break;

        case 2:
            profile = "4G";
            break;

        case 3:
            profile = "Wi-Fi";
            break;

        case 4:
            profile = "Satellite";
            break;

        default:
            std::cerr << "Invalid profile choice.\n";
            return 1;
    }

    chaos_config config = get_profile(profile);

    std::cout << "\nSelected Profile: "
              << profile << "\n";

    std::cout << "Latency: "
              << config.latency << " ms\n";

    std::cout << "Packet Loss: "
              << config.packet_loss << "%\n";

    std::cout << "Jitter: "
              << config.jitter << " ms\n";


    int fd = open(DEVICE_PATH, O_RDWR);

    if (fd < 0)
    {
        std::cerr << "\nError: Cannot open "
                  << DEVICE_PATH << "\n";

        std::cerr << "Reason: "
                  << std::strerror(errno) << "\n";

        return 1;
    }

    std::cout << "\nLinux device driver connected successfully.\n";


    if (ioctl(fd, CHAOS_IOCTL_SET_CONFIG, &config) < 0)
    {
        std::cerr << "Error: Failed to send configuration to driver.\n";
        close(fd);
        return 1;
    }

    std::cout << "Configuration sent to driver.\n";


    chaos_config received_config{};

    if (ioctl(fd, CHAOS_IOCTL_GET_CONFIG,
              &received_config) < 0)
    {
        std::cerr << "Error: Failed to read configuration from driver.\n";
        close(fd);
        return 1;
    }

    std::cout << "\nConfiguration received from driver:\n";

    std::cout << "Latency: "
              << received_config.latency << " ms\n";

    std::cout << "Packet Loss: "
              << received_config.packet_loss << "%\n";

    std::cout << "Jitter: "
              << received_config.jitter << " ms\n";


    std::cout << "\nApplying network configuration...\n";

    if (apply_network_config(received_config))
    {
        std::cout << "Network profile applied successfully.\n";
        std::cout << "Interface: "
                  << NETWORK_INTERFACE << "\n";
    }
    else
    {
        std::cerr << "Failed to apply network configuration.\n";
        close(fd);
        return 1;
    }

    close(fd);

    std::cout << "\n====================================\n";
    std::cout << " Profile configuration successful.\n";
    std::cout << "====================================\n";

    return 0;
}
