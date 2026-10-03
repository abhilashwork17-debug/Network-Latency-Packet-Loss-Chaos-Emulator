# Stage 6 – Final Implementation and Presentation

## 1. Introduction

Stage 6 focuses on the final implementation, verification, documentation and presentation of the Network Latency and Packet Loss Chaos Emulator.

The final project combines a C++ user-space application with a Linux character device driver and Linux networking tools.

The project implements the following seven features:

- Latency / Delay
- Packet Loss
- Jitter
- Network Profiles
- TCP Testing
- UDP Testing
- Linux Device Driver

---

## 2. Final System Architecture

The final system consists of the following major components:

```text
                 User
                   |
                   v
        +----------------------+
        |   C++ Chaos          |
        |     Emulator         |
        +----------------------+
                   |
                   v
        +----------------------+
        | Network Profile      |
        | Selection            |
        +----------------------+
                   |
                   v
        +----------------------+
        | Linux Character      |
        | Device Driver        |
        | /dev/chaos_driver    |
        +----------------------+
                   |
             IOCTL / Device I/O
                   |
                   v
        +----------------------+
        | Network Configuration |
        | Latency / Loss/Jitter|
        +----------------------+
                   |
                   v
        +----------------------+
        | Linux tc netem       |
        +----------------------+
                   |
                   v
        +----------------------+
        | enp0s3 Network       |
        | Interface            |
        +----------------------+
                   |
                   v
              TCP / UDP
                 Tests
```

---

## 3. Final Feature Implementation

The final implementation contains the seven features defined in the project scope:

- Latency / Delay
- Packet Loss
- Jitter
- Network Profiles
- TCP Testing
- UDP Testing
- Linux Device Driver

Each feature was implemented and tested as part of the final prototype.

---

## 4. Final Implementation Workflow

The final execution workflow is:

```text
User
  |
  v
C++ Chaos Emulator
  |
  v
Select Network Profile
  |
  v
Linux Character Device Driver
  |
  v
IOCTL Configuration
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

---

## 5. Final Testing Summary

The following components were successfully tested:

- Linux Device Driver
- C++ Driver Communication
- IOCTL SET Configuration
- IOCTL GET Configuration
- Latency / Delay
- Packet Loss
- Jitter
- 3G Network Profile
- 4G Network Profile
- Wi-Fi Network Profile
- Satellite Network Profile
- TCP Testing
- UDP Testing
- `tc netem` Integration

The four network profiles were successfully applied and verified on the `enp0s3` interface.

The TCP and UDP programs successfully demonstrated client-server communication.

---

## 6. Source Code Organization

The final project follows this structure:

```text
Network-Latency-Packet-Loss-Chaos-Emulator/
│
├── driver/
│   ├── chaos_driver.c
│   └── Makefile
│
├── include/
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

The actual project implementation uses C and C++.

---

## 7. Build and Execution

### Linux Device Driver

The driver can be compiled using:

```bash
make -C /lib/modules/$(uname -r)/build M=$(pwd)/driver modules
```

The module can be loaded using:

```bash
sudo insmod driver/chaos_driver.ko
```

The device can be verified using:

```bash
ls -l /dev/chaos_driver
```

### C++ Application

The main application can be compiled using:

```bash
g++ -Wall -Wextra src/main.cpp -o chaos_emulator
```

Run the application using:

```bash
sudo ./chaos_emulator
```

### TCP Test

Compile the TCP test:

```bash
g++ -Wall -Wextra tests/tcp_test.cpp -o tests/tcp_test
```

Run the TCP server:

```bash
./tests/tcp_test server
```

Run the TCP client from another terminal:

```bash
./tests/tcp_test client 127.0.0.1
```

### UDP Test

Compile the UDP test:

```bash
g++ -Wall -Wextra tests/udp_test.cpp -o tests/udp_test
```

Run the UDP server:

```bash
./tests/udp_test server
```

Run the UDP client:

```bash
./tests/udp_test client 127.0.0.1
```

---

## 8. Network Configuration Verification

The currently applied network configuration can be checked using:

```bash
sudo tc qdisc show dev enp0s3
```

After testing, the network impairment configuration can be removed using:

```bash
sudo tc qdisc del dev enp0s3 root
```

---

## 9. Git Version Control

Git was used throughout the development process to maintain version history.

The project was developed using incremental commits for the different development stages.

The repository contains:

- Source code
- Linux driver code
- Test programs
- Documentation
- Build files
- Project structure

Git provides a history of the development and helps maintain different stages of the project.

---

## 10. Final Limitations

The TCP and UDP functional tests were performed using endpoints within the same Ubuntu virtual machine.

The local test traffic was routed through the Linux loopback interface (`lo`) instead of the `enp0s3` interface.

Therefore, the TCP and UDP tests demonstrate successful socket communication, but their measured local round-trip times do not demonstrate the effect of `tc netem` on `enp0s3`.

A separate network endpoint would be required for complete end-to-end validation of network impairment effects on TCP and UDP traffic.

---

## 11. Final Project Status

The Network Latency and Packet Loss Chaos Emulator has reached a functional prototype suitable for final demonstration.

The project demonstrates the integration of:

- C++ system programming
- Linux character device driver
- IOCTL communication
- Linux networking
- `tc netem`
- TCP sockets
- UDP sockets
- Network condition profiles

The project documentation covers the development process from project introduction and requirements through system design, implementation, testing and final presentation.

---

## 12. Presentation Plan

The final demonstration can be completed within 5–10 minutes.

The presentation flow will be:

1. Introduce the problem and project objective.
2. Explain the system architecture.
3. Show the Linux device driver.
4. Run the C++ Chaos Emulator.
5. Select a network profile.
6. Show the driver configuration.
7. Verify the `tc netem` configuration.
8. Demonstrate TCP testing.
9. Demonstrate UDP testing.
10. Explain testing results and limitations.
11. Conclude with the project scope and implementation.

---

## 13. Conclusion

The Network Latency and Packet Loss Chaos Emulator provides a Linux-based environment for configuring and testing network conditions such as latency, packet loss and jitter.

The project integrates a C++ user-space application with a Linux character device driver and the Linux `tc netem` networking subsystem.

The final implementation demonstrates practical concepts from Linux system programming, device drivers, socket programming and network configuration.

The project provides a structured foundation for network testing and demonstrates the practical integration of Linux kernel and user-space programming.
