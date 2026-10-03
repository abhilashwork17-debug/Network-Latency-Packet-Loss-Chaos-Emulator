#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <string>

#define TCP_PORT 5000
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3)
    {
        std::cout << "Usage:\n";
        std::cout << "  Server: ./tcp_test server\n";
        std::cout << "  Client: ./tcp_test client <server-ip>\n";
        return 1;
    }

    std::string mode = argv[1];

    // =========================
    // TCP SERVER
    // =========================
    if (mode == "server")
    {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (server_fd < 0)
        {
            perror("socket");
            return 1;
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(TCP_PORT);

        int option = 1;

        setsockopt(
            server_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &option,
            sizeof(option)
        );

        if (bind(
                server_fd,
                (struct sockaddr*)&address,
                sizeof(address)) < 0)
        {
            perror("bind");
            close(server_fd);
            return 1;
        }

        if (listen(server_fd, 5) < 0)
        {
            perror("listen");
            close(server_fd);
            return 1;
        }

        std::cout << "TCP server listening on port "
                  << TCP_PORT << "...\n";

        std::cout << "Waiting for clients...\n";

        // Keep server running
        while (true)
        {
            int client_fd = accept(
                server_fd,
                nullptr,
                nullptr
            );

            if (client_fd < 0)
            {
                perror("accept");
                continue;
            }

            std::cout << "\nClient connected.\n";

            char buffer[BUFFER_SIZE];

            ssize_t bytes = read(
                client_fd,
                buffer,
                BUFFER_SIZE - 1
            );

            if (bytes > 0)
            {
                buffer[bytes] = '\0';

                std::cout << "Received: "
                          << buffer << "\n";

                send(
                    client_fd,
                    buffer,
                    bytes,
                    0
                );

                std::cout << "Response sent to client.\n";
            }
            else if (bytes == 0)
            {
                std::cout << "Client disconnected.\n";
            }
            else
            {
                perror("read");
            }

            close(client_fd);

            std::cout << "Waiting for next client...\n";
        }

        close(server_fd);
    }

    // =========================
    // TCP CLIENT
    // =========================
    else if (mode == "client")
    {
        if (argc != 3)
        {
            std::cout << "Usage: ./tcp_test client <server-ip>\n";
            return 1;
        }

        const char* server_ip = argv[2];

        int client_fd = socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

        if (client_fd < 0)
        {
            perror("socket");
            return 1;
        }

        sockaddr_in server_address{};

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(TCP_PORT);

        if (inet_pton(
                AF_INET,
                server_ip,
                &server_address.sin_addr) <= 0)
        {
            std::cerr << "Invalid server IP address.\n";
            close(client_fd);
            return 1;
        }

        std::cout << "Connecting to TCP server...\n";

        if (connect(
                client_fd,
                (struct sockaddr*)&server_address,
                sizeof(server_address)) < 0)
        {
            perror("connect");
            close(client_fd);
            return 1;
        }

        std::cout << "Connected successfully.\n";

        const char* message =
            "TCP test message from Chaos Emulator";

        auto start =
            std::chrono::steady_clock::now();

        ssize_t sent = send(
            client_fd,
            message,
            strlen(message),
            0
        );

        if (sent < 0)
        {
            perror("send");
            close(client_fd);
            return 1;
        }

        char buffer[BUFFER_SIZE];

        ssize_t bytes = read(
            client_fd,
            buffer,
            BUFFER_SIZE - 1
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

            std::cout << "\nTCP response received: "
                      << buffer << "\n";

            std::cout << "Round-trip time: "
                      << elapsed.count()
                      << " ms\n";
        }
        else if (bytes == 0)
        {
            std::cout << "Server closed the connection.\n";
        }
        else
        {
            perror("read");
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
