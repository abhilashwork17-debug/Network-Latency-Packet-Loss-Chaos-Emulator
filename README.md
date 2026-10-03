# Network Latency and Packet Loss Chaos Emulator

A Linux-based network chaos emulator developed using C++ and a Linux character device driver.

The project is designed to simulate selected network conditions such as latency, packet loss and jitter, while providing TCP and UDP testing capabilities.

## Features

- Latency / Delay
- Packet Loss
- Jitter
- Network Profiles
- TCP Testing
- UDP Testing
- Linux Device Driver

---

## Project Objective

The objective of this project is to provide a Linux-based environment for simulating selected network conditions and testing TCP and UDP communication.

The system combines a C++ user-space application, a Linux character device driver and the Linux `tc netem` networking subsystem.

The Linux device driver receives and stores the selected network configuration through IOCTL communication.

The C++ application then applies the configuration to the selected network interface using `tc netem`.

---

## System Architecture

```text
User
  |
  v
C++ Chaos Emulator
  |
  v
Network Profile Selection
  |
  v
Linux Character Device Driver
  |
  | IOCTL
  v
Network Configuration
  |
  v
tc netem
  |
  v
enp0s3 Network Interface
  |
  v
TCP / UDP Testing
```

## Network Profiles

| Profile | Latency | Jitter | Packet Loss |
|---|---:|---:|---:|
| 3G | 100 ms | 30 ms | 2% |
| 4G | 50 ms | 15 ms | 1% |
| Wi-Fi | 20 ms | 5 ms | 1% |
| Satellite | 600 ms | 50 ms | 2% |

These values are project-defined simulation parameters.

---

## Project Structure

```text
Network-Latency-Packet-Loss-Chaos-Emulator/
│
├── driver/
│   ├── chaos_driver.c
│   └── Makefile
│
├── src/
│   └── main.cpp
│
├── tests/
│   ├── driver_ioctl_test.cpp
│   ├── tcp_test.cpp
│   └── udp_test.cpp
│
├── docs/
│   ├── Stage1/
│   ├── Stage2/
│   ├── Stage3/
│   ├── Stage4/
│   ├── Stage5/
│   └── Stage6/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

## Technologies Used

- C
- C++
- Linux Kernel Module
- Linux Character Device Driver
- IOCTL
- TCP Sockets
- UDP Sockets
- `tc netem`
- Git
- Make
- CMake

---

## Requirements

The project requires:

- Ubuntu Linux
- GCC / G++
- Linux kernel headers
- Make
- CMake
- Git
- `iproute2` / `tc`

The Linux kernel headers should match the running kernel version.

---

## Build and Execution

### 1. Build the Linux Device Driver

From the project root:

```bash
make -C /lib/modules/$(uname -r)/build M=$(pwd)/driver modules
```

### 2. Load the Driver

```bash
sudo insmod driver/chaos_driver.ko
```

Verify the device:

```bash
ls -l /dev/chaos_driver
```

### 3. Build the Main Application

```bash
g++ -Wall -Wextra src/main.cpp -o chaos_emulator
```

### 4. Run the Main Application

```bash
sudo ./chaos_emulator
```

The application allows the user to select one of the predefined network profiles.

---

## Driver IOCTL Test

Compile the driver communication test:

```bash
g++ tests/driver_ioctl_test.cpp -o tests/driver_ioctl_test
```

Run it using:

```bash
sudo ./tests/driver_ioctl_test
```

The test sends a configuration to the Linux device driver and retrieves the configuration using IOCTL.

---

## TCP Testing

Compile the TCP test:

```bash
g++ -Wall -Wextra tests/tcp_test.cpp -o tests/tcp_test
```

Start the TCP server:

```bash
./tests/tcp_test server
```

In another terminal, run the TCP client:

```bash
./tests/tcp_test client 127.0.0.1
```

The client sends a message to the server and receives a response while measuring the round-trip time.

---

## UDP Testing

Compile the UDP test:

```bash
g++ -Wall -Wextra tests/udp_test.cpp -o tests/udp_test
```

Start the UDP server:

```bash
./tests/udp_test server
```

In another terminal, run the UDP client:

```bash
./tests/udp_test client 127.0.0.1
```

The client sends a UDP packet and receives a response from the server.

---

## Network Configuration Verification

The currently applied network configuration can be checked using:

```bash
sudo tc qdisc show dev enp0s3
```

After testing, remove the network impairment configuration using:

```bash
sudo tc qdisc del dev enp0s3 root
```

---

## Documentation

The project documentation is divided into six development stages:

### Stage 1
Project Introduction

### Stage 2
Requirements and Development Plan

### Stage 3
System Design and Architecture

### Stage 4
Initial Implementation and Prototype

### Stage 5
Testing, Integration and Improvement

### Stage 6
Final Implementation and Presentation

All stage documentation is available inside the `docs/` directory.

---

## Testing Status

The following components were tested:

- Linux Device Driver
- C++ Driver Communication
- IOCTL SET Configuration
- IOCTL GET Configuration
- Latency / Delay
- Packet Loss
- Jitter
- 3G Profile
- 4G Profile
- Wi-Fi Profile
- Satellite Profile
- TCP Testing
- UDP Testing
- `tc netem` Integration

The four network profiles were successfully applied and verified on the `enp0s3` interface.

---

## Testing Limitation

The TCP and UDP functional tests were performed using endpoints within the same Ubuntu virtual machine.

The local test traffic was routed through the Linux loopback interface (`lo`) rather than the `enp0s3` interface.

Therefore, the TCP and UDP tests demonstrate successful socket communication, but their local round-trip measurements do not demonstrate the effect of `tc netem` on `enp0s3`.

A separate network endpoint would be required for complete end-to-end validation of network impairment effects on TCP and UDP traffic.

---

## Git Version Control

Git was used throughout the development process to maintain version history.

The repository contains:

- Source code
- Linux driver code
- Test programs
- Documentation
- Build files
- Project structure

Development was maintained through incremental Git commits for the different project stages.

---

## Cleanup

After testing, remove the network impairment:

```bash
sudo tc qdisc del dev enp0s3 root
```

To unload the Linux device driver:

```bash
sudo rmmod chaos_driver
```

---

## Project Status

The Network Latency and Packet Loss Chaos Emulator has reached a functional prototype suitable for final demonstration.

The project demonstrates practical concepts from:

- Linux system programming
- Linux device drivers
- C and C++ programming
- IOCTL communication
- TCP socket programming
- UDP socket programming
- Linux network configuration

---

## Author

Abhinab Kumar Das

B.Tech – Computer Science and Engineering (IoT)

Siksha 'O' Anusandhan University

---

## License

This project is developed for academic and educational purposes.
