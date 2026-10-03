# Stage 1 – Project Introduction

## 1. Project Title

# Network Latency and Packet Loss Chaos Emulator

## 2. Introduction

Network performance can vary significantly depending on the type and quality of the connection. Applications that perform normally on a stable Wi-Fi network may behave differently when operating through a slower or unstable network.

The **Network Latency and Packet Loss Chaos Emulator** is a Linux-based project designed to create controlled network conditions for testing and observation. The system allows selected network impairments to be introduced deliberately so that their effect on network communication can be studied in a repeatable environment.

The project focuses on three fundamental network conditions: **latency, packet loss, and jitter**. These conditions can be configured individually or through predefined network profiles representing **3G, 4G, Wi-Fi, and satellite** environments.

The system also provides basic **TCP and UDP testing** to observe how different transport protocols behave when network conditions are changed.

A major component of the project is a **Linux character device driver**, which provides the kernel-level component of the system. A C++ application will be used as the user-space control component for configuring and interacting with the emulator.

---

## 3. Problem Statement

Network applications are commonly tested under relatively stable network conditions. However, real networks can experience delay, packet loss, and variations in packet delivery time.

Testing these conditions using real networks can be difficult because the conditions are not always predictable or repeatable. Different network technologies also have different characteristics.

For example, a program may work normally on Wi-Fi but experience increased delay when operating over a high-latency satellite connection.

Therefore, a controlled Linux-based environment is required to reproduce selected network conditions and observe their effect on TCP and UDP communication.

---

## 4. Project Objective

The main objective of this project is to develop a Linux-based network chaos emulator that can reproduce selected network conditions in a controlled environment.

The project aims to:

1. Introduce configurable network latency or delay.
2. Simulate packet loss.
3. Simulate network jitter.
4. Provide predefined profiles for 3G, 4G, Wi-Fi, and satellite networks.
5. Perform basic TCP testing under different network conditions.
6. Perform basic UDP testing under different network conditions.
7. Develop and integrate a Linux character device driver.
8. Provide a C++ application for controlling the emulator.

---

## 5. Project Scope

The project is limited to the following seven core features:

### 5.1 Latency / Delay

The system will allow a configurable delay to be introduced into network communication.

### 5.2 Packet Loss

The system will allow a selected percentage of packets to be dropped during testing.

### 5.3 Jitter

The system will introduce variation in network delay to simulate unstable packet delivery timing.

### 5.4 Network Profiles

The system will provide predefined configurations representing:

- 3G
- 4G
- Wi-Fi
- Satellite

Each profile will contain selected latency, packet-loss, and jitter parameters.

### 5.5 TCP Testing

The system will provide a basic TCP testing mechanism to observe TCP communication under different network conditions.

### 5.6 UDP Testing

The system will provide a basic UDP testing mechanism to observe UDP communication under different network conditions.

### 5.7 Linux Device Driver

A Linux character device driver will be developed as the kernel-level component of the project. The C++ application will communicate with the driver through Linux system-programming interfaces.

---

## 6. Proposed System

The proposed system consists of three main layers.

### User Space

The user interacts with a C++ application to select network conditions and testing options.

### Kernel Space

A Linux character device driver provides the kernel-level interface for receiving and managing the required configuration.

### Network Layer

Linux networking facilities are used to apply the configured latency, packet loss, and jitter to network traffic.

The overall flow is:

    User
      ↓
    C++ Application
      ↓
    Linux Character Device Driver
      ↓
    Linux Network Subsystem
      ↓
    Network
      ↓
    TCP / UDP Testing

---

## 7. Expected Outcome

The expected outcome is a functional Linux-based network testing system capable of applying selected network conditions and testing TCP and UDP communication.

A user should be able to select a profile such as:

    Wi-Fi

or configure individual parameters such as:

    Latency: 100 ms
    Packet Loss: 5%
    Jitter: 20 ms

The system will then apply the selected configuration and allow the user to perform TCP or UDP testing.

The project will demonstrate how changes in network conditions can affect communication behaviour.

---

## 8. Applications

The project can be used for:

- Network application testing
- TCP and UDP experiments
- Network-condition simulation
- Linux networking education
- Software reliability experiments
- Academic networking laboratories
- Understanding the effect of latency, packet loss, and jitter

---

## 9. Technologies Used

| Component | Technology |
|---|---|
| Operating System | Ubuntu Linux |
| Application Language | C++ |
| Device Driver | C |
| Build Tools | Make / CMake |
| Version Control | Git |
| Network Control | Linux Traffic Control |
| Transport Protocols | TCP / UDP |
| Kernel Interface | Linux Character Device |

---

## 10. Project Limitations

The project focuses specifically on latency, packet loss, jitter, network profiles, TCP testing, UDP testing, and Linux device-driver integration.

Other network impairment techniques are outside the scope of the current implementation.

The predefined network profiles are project-defined configurations intended for controlled experimentation and are not intended to represent exact measurements for every real-world network.

---

## 11. Expected Learning Outcomes

The project will provide practical experience in:

- Linux system programming
- C and C++ programming
- Linux character device drivers
- Kernel-space and user-space communication
- Linux networking
- TCP and UDP communication
- Network latency and packet-loss behaviour
- Jitter and network variability
- Object-oriented programming
- Git-based development
- Software architecture and testing
