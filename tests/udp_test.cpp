#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <string>

#define UDP_PORT 5001
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3)
    {
        std::cout << "Usage:\n";
        std::cout << "  Server: ./udp_test server\n";
        std::cout << "  Client: ./udp_test client <server-ip>\n";
        return 1;
    }

    std::string mode = argv[1];

    // =========================
    // UDP SERVER
    // =========================
    if (mode == "server")
    {
        int server_fd = socket(
            AF_INET,
            SOCK_DGRAM,
            0
        );

        if (server_fd < 0)
        {
            perror("socket");
            return 1;
        }

        sockaddr_in server_address{};

        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(UDP_PORT);

        if (bind(
                server_fd,
                (struct sockaddr*)&server_address,
                sizeof(server_address)) < 0)
        {
            perror("bind");
            close(server_fd);
            return 1;
        }

        std::cout << "UDP server listening on port "
                  << UDP_PORT << "...\n";

        std::cout << "Waiting for UDP packets...\n";

        while (true)
        {
            char buffer[BUFFER_SIZE];

            sockaddr_in client_address{};
            socklen_t client_length =
                sizeof(client_address);

            ssize_t bytes = recvfrom(
                server_fd,
                buffer,
                BUFFER_SIZE - 1,
                0,
                (struct sockaddr*)&client_address,
                &client_length
            );

            if (bytes < 0)
            {
                perror("recvfrom");
                continue;
            }

            buffer[bytes] = '\0';

            std::cout << "\nReceived: "
                      << buffer << "\n";

            ssize_t sent = sendto(
                server_fd,
                buffer,
                bytes,
                0,
                (struct sockaddr*)&client_address,
                client_length
            );

            if (sent < 0)
            {
                perror("sendto");
            }
            else
            {
                std::cout << "UDP response sent.\n";
            }
        }

        close(server_fd);
    }

    // =========================
    // UDP CLIENT
    // =========================
    else if (mode == "client")
    {
        if (argc != 3)
        {
            std::cout << "Usage: ./udp_test client <server-ip>\n";
            return 1;
        }

        const char* server_ip = argv[2];

        int client_fd = socket(
            AF_INET,
            SOCK_DGRAM,
            0
        );

        if (client_fd < 0)
        {
            perror("socket");
            return 1;
        }

        sockaddr_in server_address{};

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(UDP_PORT);

        if (inet_pton(
                AF_INET,
                server_ip,
                &server_address.sin_addr) <= 0)
        {
            std::cerr << "Invalid server IP address.\n";
            close(client_fd);
            return 1;
        }

        const char* message =
            "UDP test message from Chaos Emulator";

        std::cout << "Sending UDP packet...\n";

        auto start =
            std::chrono::steady_clock::now();

        ssize_t sent = sendto(
            client_fd,
            message,
            strlen(message),
            0,
            (struct sockaddr*)&server_address,
            sizeof(server_address)
        );

        if (sent < 0)
        {
            perror("sendto");
            close(client_fd);
            return 1;
        }

        char buffer[BUFFER_SIZE];

        sockaddr_in response_address{};
        socklen_t response_length =
            sizeof(response_address);

        ssize_t bytes = recvfrom(
            client_fd,
            buffer,
            BUFFER_SIZE - 1,
            0,
            (struct sockaddr*)&response_address,
            &response_length
        );

        auto end =
            std::chrono::steady_clock::now();

        if (bytes > 0)
        {
            buffer[bytes] = '\0';

            auto elapsed =
                std::chrono::duration_cast<
                    std::chrono::milliseconds
                >(end - start);

            std::cout << "\nUDP response received: "
                      << buffer << "\n";

            std::cout << "Round-trip time: "
                      << elapsed.count()
                      << " ms\n";
        }
        else if (bytes == 0)
        {
            std::cout << "Empty UDP response received.\n";
        }
        else
        {
            perror("recvfrom");
        }

        close(client_fd);
    }

    else
    {
        std::cout << "Invalid mode.\n";
        std::cout << "Use 'server' or 'client'.\n";
        return 1;
    }

    return 0;
}
