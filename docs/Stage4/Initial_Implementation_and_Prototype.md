# Stage 4 – Initial Implementation and Prototype

## 1. Introduction

Stage 4 focuses on implementing the initial working prototype of the Network Latency and Packet Loss Chaos Emulator.

The prototype was developed using C++ for the user-space application and C for the Linux character device driver.

The implementation currently supports:

- Latency / Delay
- Packet Loss
- Jitter
- Network Profiles
- TCP Testing
- UDP Testing
- Linux Device Driver

---

## 2. Linux Device Driver Implementation

A Linux character device driver was implemented as:

```text
/dev/chaos_driver
The driver provides:

- Device open and release operations
- Read and write operations
- IOCTL-based configuration
- Configuration validation
- Kernel-side configuration storage
- Synchronization using a mutex

The configuration contains:

- Latency
- Packet Loss
- Jitter

The driver was successfully loaded into the Linux kernel and the device node was created successfully.

---

## 3. C++ Driver Communication

The C++ application communicates with the Linux device driver using `open()` and `ioctl()` system calls.

The communication flow is:

```text
C++ Application
      |
      | open()
      v
/dev/chaos_driver
      |
      | ioctl()
      v
Linux Character Device Driver
      |
      v
Kernel Configuration

The driver was tested by sending the following configuration:

Latency: 100 ms
Packet Loss: 5%
Jitter: 20 ms

The same configuration was successfully retrieved from the driver.

Example output:

Configuration sent to driver:
Latency: 100 ms
Packet Loss: 5%
Jitter: 20 ms

Configuration received from driver:
Latency: 100 ms
Packet Loss: 5%
Jitter: 20 ms

Driver communication successful.
---

## 4. Network Impairment Implementation

Linux `tc netem` was integrated with the C++ application to apply network conditions to the `enp0s3` interface.

The supported parameters are:

- Delay
- Jitter
- Packet Loss

Example configuration:

delay 100ms 20ms loss 5%

The configuration was verified using:

sudo tc qdisc show dev enp0s3

The verification confirmed that the configured delay, jitter, and packet loss values were successfully applied to the network interface.
---

## 5. Network Profiles

Four predefined network profiles were implemented:

| Profile | Latency | Jitter | Packet Loss |
|---|---:|---:|---:|
| 3G | 100 ms | 30 ms | 2% |
| 4G | 50 ms | 15 ms | 1% |
| Wi-Fi | 20 ms | 5 ms | 1% |
| Satellite | 600 ms | 50 ms | 2% |

These values are project-defined simulation parameters.

All four profiles were successfully applied and verified using `tc netem`.
---

## 6. TCP Testing

A C++ TCP client/server test was implemented.

The TCP test supports:

- TCP server
- TCP client
- Data transmission
- Response reception
- Round-trip time measurement

The TCP client and server successfully established a connection and exchanged data.

Example output:

TCP response received: TCP test message from Chaos Emulator
Round-trip time: 1 ms
---

## 7. UDP Testing

A C++ UDP client/server test was implemented.

The UDP test supports:

- UDP server
- UDP client
- UDP packet transmission
- Response reception
- Round-trip time measurement

The UDP client and server successfully exchanged packets.

Example output:

UDP response received: UDP test message from Chaos Emulator
Round-trip time: 1 ms
---

## 8. Prototype Execution Flow

The current prototype follows this execution flow:

User
  |
  v
C++ Chaos Emulator
  |
  |-- Select Network Profile
  |
  v
Linux Character Device Driver
  |
  |-- Store Configuration
  |
  v
C++ Network Controller
  |
  v
tc netem
  |
  v
enp0s3
  |
  +---- TCP Testing
  |
  +---- UDP Testing
  ---

## 9. Development Environment

The prototype was developed using:

- Operating System: Ubuntu Linux
- Kernel Version: 7.0.0-38-generic
- Programming Languages: C and C++
- Compiler: GCC / G++
- Build Tools: Make and CMake
- Version Control: Git
- Network Tool: iproute2 / tc
- Virtualization: Oracle VirtualBox
---

## 10. Prototype Status

The initial prototype successfully demonstrates:

- Linux character device driver operation
- C++ to kernel communication
- IOCTL configuration
- Latency configuration
- Packet loss configuration
- Jitter configuration
- Network profile selection
- `tc netem` integration
- TCP socket testing
- UDP socket testing

The four network profiles were successfully applied and verified on the `enp0s3` interface.

Stage 4 establishes the first functional prototype. Further testing and validation will be documented in Stage 5.
