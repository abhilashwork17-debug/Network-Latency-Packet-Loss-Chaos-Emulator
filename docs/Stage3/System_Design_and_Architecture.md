# Stage 3 – System Design & Architecture

## 1. Introduction

Stage 3 defines the system architecture and technical design of the Network Latency and Packet Loss Chaos Emulator.

The purpose of this stage is to describe how the seven core features are organized and how the user-space C++ application, Linux character device driver, and Linux networking components communicate with each other.

The system is divided into user space, kernel space, and the Linux networking layer.

The seven core features are:

1. Latency / Delay
2. Packet Loss
3. Jitter
4. Network Profiles
5. TCP Testing
6. UDP Testing
7. Linux Device Driver

The design is based entirely on C and C++ running on Ubuntu Linux.

---

## 2. Overall System Architecture

The system follows a layered architecture consisting of user-space software, a Linux kernel-space device driver, and the Linux networking subsystem.

### 2.1 User Space

The user interacts with the C++ control application.

The main C++ application is implemented in:

```text
src/main.cpp
```

The application is responsible for:

- Selecting a predefined network profile
- Creating the latency, packet loss, and jitter configuration
- Opening the Linux character device
- Sending configuration to the driver using `ioctl()`
- Reading the configuration back from the driver
- Applying the network configuration using Linux `tc netem`
- Displaying operation status

The TCP and UDP tests are implemented as separate C++ programs:

```text
tests/tcp_test.cpp
tests/udp_test.cpp
```

### 2.2 Kernel Space

The Linux character device driver operates in kernel space.

The driver is implemented in:

```text
driver/chaos_driver.c
```

The driver provides:

- Character device registration
- Device interface through `/dev/chaos_driver`
- Configuration storage
- Configuration validation
- `read()` support
- `write()` support
- `ioctl()` support
- Communication between user space and kernel space

The driver supports two IOCTL operations:

```text
CHAOS_IOCTL_SET_CONFIG
CHAOS_IOCTL_GET_CONFIG
```

### 2.3 Linux Network Layer

The Linux networking subsystem is responsible for applying the selected network conditions.

The project uses:

```text
tc netem
```

to configure:

- Latency / Delay
- Packet Loss
- Jitter

The current network interface used by the project is:

```text
enp0s3
```

The C++ application executes `tc` from user space using `fork()` and `execl()`.

The Linux device driver does not directly execute `tc` commands. It stores and validates the configuration received through IOCTL communication.

---

## 3. System Architecture Flow

The implemented system follows this flow:

```text
                         USER
                           |
                           v
                +----------------------+
                | C++ Chaos Emulator   |
                |     main.cpp         |
                +----------+-----------+
                           |
                           | open()
                           | ioctl()
                           v
                +----------------------+
                | /dev/chaos_driver    |
                +----------+-----------+
                           |
                           v
                +----------------------+
                | Linux Character      |
                | Device Driver        |
                | chaos_driver.c       |
                +----------+-----------+
                           |
                           | Store / Validate
                           | Configuration
                           v
                +----------------------+
                | C++ Application       |
                | Receives Configuration|
                +----------+-----------+
                           |
                           | fork() + execl()
                           v
                +----------------------+
                | Linux tc netem       |
                +----------+-----------+
                           |
                           v
                +----------------------+
                | enp0s3 Network       |
                | Interface             |
                +----------+-----------+
                           |
                           v
                        NETWORK
```

The TCP and UDP test programs are separate user-space programs:

```text
+----------------------+       +----------------------+
| tests/tcp_test.cpp   |       | tests/udp_test.cpp   |
| TCP Client / Server  |       | UDP Client / Server  |
+----------------------+       +----------------------+
```

The overall configuration workflow is:

```text
Select Profile
      |
      v
Create Configuration
      |
      v
Open /dev/chaos_driver
      |
      v
IOCTL SET Configuration
      |
      v
Driver Validates and Stores
      |
      v
IOCTL GET Configuration
      |
      v
C++ Application Receives Configuration
      |
      v
Execute tc netem
      |
      v
Apply Configuration to enp0s3
```

---

## 4. Component Architecture

The final project consists of the following major components.

### 4.1 Linux Device Driver

The Linux device driver is implemented as a kernel module using C.

Source file:

```text
driver/chaos_driver.c
```

Main responsibilities:

- Register the character device
- Allocate a device number
- Create the device class
- Create `/dev/chaos_driver`
- Implement `open()`
- Implement `read()`
- Implement `write()`
- Implement `ioctl()`
- Validate configuration values
- Store the current configuration
- Provide communication between user space and kernel space
- Cleanly unload the device

The driver uses a mutex to protect configuration data.

The driver configuration contains:

```text
latency
packet_loss
jitter
```

### 4.2 C++ Control Application

The main C++ application is implemented in:

```text
src/main.cpp
```

Its responsibilities are:

- Display the application menu
- Accept profile selection
- Create the network configuration
- Open `/dev/chaos_driver`
- Send configuration using IOCTL
- Retrieve configuration using IOCTL
- Apply the network configuration
- Display the result

The application uses:

```text
open()
ioctl()
close()
fork()
execl()
waitpid()
```

for system-level operations.

### 4.3 Network Configuration Controller

The network configuration functionality is implemented directly in:

```text
src/main.cpp
```

It uses Linux:

```text
tc qdisc replace
```

with:

```text
netem
```

to apply:

- Delay
- Jitter
- Packet Loss

The current network interface is:

```text
enp0s3
```

The command structure used by the application is conceptually:

```text
tc qdisc replace dev enp0s3 root netem
    delay <latency>ms <jitter>ms
    loss <packet_loss>%
```

The C++ application launches this command using `fork()` and `execl()`.

### 4.4 Network Profiles

The application contains four predefined network profiles.

The profiles are:

```text
3G
4G
Wi-Fi
Satellite
```

The project-defined values are:

| Profile | Latency | Jitter | Packet Loss |
|---|---:|---:|---:|
| 3G | 100 ms | 30 ms | 2% |
| 4G | 50 ms | 15 ms | 1% |
| Wi-Fi | 20 ms | 5 ms | 1% |
| Satellite | 600 ms | 50 ms | 2% |

These values are simulation parameters defined for this project.

The profile selection is handled by the `get_profile()` function in `src/main.cpp`.

### 4.5 TCP Test Program

The TCP test is implemented separately in:

```text
tests/tcp_test.cpp
```

The program supports:

- TCP server mode
- TCP client mode
- TCP socket creation
- Connection establishment
- Message transmission
- Response reception
- Round-trip time measurement

The program can be executed as:

```text
./tests/tcp_test server
```

or:

```text
./tests/tcp_test client <server-ip>
```

### 4.6 UDP Test Program

The UDP test is implemented separately in:

```text
tests/udp_test.cpp
```

The program supports:

- UDP server mode
- UDP client mode
- UDP socket creation
- Datagram transmission
- Response reception
- Round-trip time measurement

The program can be executed as:

```text
./tests/udp_test server
```

or:

```text
./tests/udp_test client <server-ip>
```

---

## 5. Data Structures

The project uses structured data to represent network configuration.

### 5.1 Network Configuration Structure

The main configuration structure is:

```text
struct chaos_config
```

It contains:

```text
latency
packet_loss
jitter
```

Conceptually:

```text
+--------------------------------+
|        chaos_config            |
+--------------------------------+
| latency      : int             |
| packet_loss  : int             |
| jitter       : int             |
+--------------------------------+
```

The same structure is used by the C++ application and Linux device driver for IOCTL communication.

The configuration is transferred using:

```text
CHAOS_IOCTL_SET_CONFIG
CHAOS_IOCTL_GET_CONFIG
```

### 5.2 Network Profile Representation

The project does not use a separate `NetworkProfile` class or structure.

Instead, the `get_profile()` function in `src/main.cpp` creates a `chaos_config` based on the selected profile.

Conceptually:

```text
Profile Name
     |
     v
get_profile()
     |
     v
chaos_config
     |
     +---- latency
     +---- packet_loss
     +---- jitter
```

### 5.3 TCP and UDP Test Data

The TCP and UDP programs use socket-related data structures provided by Linux networking APIs.

The tests use:

```text
socket()
bind()
listen()
accept()
connect()
send()
recv()
sendto()
recvfrom()
close()
```

The tests also record the elapsed time required for a request and response.

---

## 6. System Architecture Diagram

The final architecture of the project is:

```text
                              USER
                                |
                                v
                  +---------------------------+
                  |    C++ Chaos Emulator     |
                  |       src/main.cpp        |
                  +-------------+-------------+
                                |
                                | open()
                                | ioctl()
                                v
                  +---------------------------+
                  |   /dev/chaos_driver       |
                  +-------------+-------------+
                                |
                                v
                  +---------------------------+
                  | Linux Character Driver    |
                  |   driver/chaos_driver.c   |
                  +-------------+-------------+
                                |
                                | Validate &
                                | Store Config
                                v
                  +---------------------------+
                  | C++ Application            |
                  | Configuration Retrieved   |
                  +-------------+-------------+
                                |
                                | fork() + execl()
                                v
                  +---------------------------+
                  |       tc netem             |
                  | Linux Network Subsystem    |
                  +-------------+-------------+
                                |
                                v
                  +---------------------------+
                  |       enp0s3               |
                  | Network Interface          |
                  +-------------+-------------+
                                |
                                v
                             NETWORK


       Independent Socket Testing Programs

       +-----------------------+    +-----------------------+
       | tests/tcp_test.cpp    |    | tests/udp_test.cpp    |
       | TCP Client / Server   |    | UDP Client / Server   |
       +-----------------------+    +-----------------------+
```

### Architecture Responsibilities

**C++ application**

Controls the project and communicates with the driver.

**Linux device driver**

Provides the kernel-space character device interface and stores validated configuration.

**IOCTL interface**

Transfers structured configuration between user space and kernel space.

**tc netem**

Applies latency, packet loss and jitter to the selected Linux network interface.

**TCP test**

Tests TCP client/server communication.

**UDP test**

Tests UDP client/server communication.

---

## 7. UML Class and Component Design

The project uses a procedural/function-based C++ implementation rather than a class-based architecture.

Therefore, the design below represents the actual implemented components and functions rather than claiming classes that do not exist in the source code.

### 7.1 C++ Configuration Structure

```text
+--------------------------------+
|        chaos_config            |
+--------------------------------+
| latency : int                  |
| packet_loss : int              |
| jitter : int                   |
+--------------------------------+
```

### 7.2 Main Application Functions

The main C++ application contains the following major functions:

```text
+--------------------------------------+
|       src/main.cpp                   |
+--------------------------------------+
| get_profile()                        |
| apply_network_config()               |
| main()                               |
+--------------------------------------+
```

`get_profile()`:

- Selects the configuration values for 3G
- Selects the configuration values for 4G
- Selects the configuration values for Wi-Fi
- Selects the configuration values for Satellite

`apply_network_config()`:

- Creates a child process using `fork()`
- Builds the `tc netem` parameters
- Executes `/sbin/tc`
- Waits for the child process using `waitpid()`
- Reports whether the network configuration was applied successfully

`main()`:

- Displays the application
- Accepts the profile choice
- Opens the device
- Sends configuration using IOCTL
- Retrieves configuration using IOCTL
- Calls the network configuration function
- Closes the device

### 7.3 Linux Device Driver Components

```text
+--------------------------------------+
|       chaos_driver.c                 |
+--------------------------------------+
| chaos_open()                         |
| chaos_release()                      |
| chaos_read()                         |
| chaos_write()                        |
| chaos_ioctl()                        |
| chaos_driver_init()                  |
| chaos_driver_exit()                  |
+--------------------------------------+
```

The driver uses:

```text
struct chaos_config
```

for structured configuration.

### 7.4 TCP and UDP Components

The TCP and UDP test programs are separate executable components.

```text
+-------------------------+
| tests/tcp_test.cpp      |
+-------------------------+
| TCP Server              |
| TCP Client              |
| Socket Communication    |
| RTT Measurement         |
+-------------------------+

+-------------------------+
| tests/udp_test.cpp      |
+-------------------------+
| UDP Server              |
| UDP Client              |
| Datagram Communication  |
| RTT Measurement         |
+-------------------------+
```

---

## 8. UML Sequence Design

### 8.1 Main Configuration Sequence

The implemented configuration sequence is:

```text
User
 |
 | Select profile
 v
C++ Chaos Emulator
 |
 | Create chaos_config
 v
C++ Chaos Emulator
 |
 | open("/dev/chaos_driver")
 v
Linux Character Device
 |
 | ioctl(SET_CONFIG)
 v
Linux Device Driver
 |
 | Validate and store
 v
Linux Device Driver
 |
 | ioctl(GET_CONFIG)
 v
C++ Chaos Emulator
 |
 | Retrieve configuration
 v
C++ Chaos Emulator
 |
 | fork()
 | execl("/sbin/tc", ...)
 v
tc netem
 |
 | Apply delay/loss/jitter
 v
enp0s3
 |
 v
Network
```

### 8.2 TCP Test Sequence

The TCP program operates independently:

```text
TCP Server                         TCP Client
    |                                  |
    | <------ connect() ---------------|
    |                                  |
    | <------ TCP message -------------|
    |                                  |
    | -------- response -------------->|
    |                                  |
    |                          Measure RTT
    |                                  |
    +----------------------------------+
```

### 8.3 UDP Test Sequence

The UDP program operates independently:

```text
UDP Server                         UDP Client
    |                                  |
    | <------ sendto() ----------------|
    |                                  |
    | -------- response -------------->|
    |                                  |
    |                          Measure RTT
    |                                  |
    +----------------------------------+
```

The TCP and UDP tests demonstrate functional socket communication.

During testing, local endpoints within the same Ubuntu virtual machine were routed through the loopback interface (`lo`). Therefore, the local RTT results do not demonstrate the effect of `tc netem` on the `enp0s3` interface.

---

## 9. UML State Machine Design

The main application follows this operational state flow:

```text
                    +---------+
                    |  START  |
                    +----+----+
                         |
                         v
                 +---------------+
                 | INITIALIZE    |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | SELECT        |
                 | NETWORK       |
                 | PROFILE       |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | CREATE        |
                 | CONFIGURATION |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | OPEN          |
                 | DEVICE        |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | SEND CONFIG   |
                 | USING IOCTL   |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | GET CONFIG    |
                 | USING IOCTL   |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | APPLY         |
                 | tc netem      |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | CONFIGURATION |
                 | APPLIED       |
                 +-------+-------+
                         |
                         v
                 +---------------+
                 | END           |
                 | / CLEANUP     |
                 +---------------+
```

### TCP Test State Flow

```text
START
  |
  v
SELECT SERVER / CLIENT
  |
  v
CREATE SOCKET
  |
  v
BIND / CONNECT
  |
  v
SEND / RECEIVE
  |
  v
MEASURE RTT
  |
  v
DISPLAY RESULT
  |
  v
CLOSE SOCKET
  |
  v
END
```

### UDP Test State Flow

```text
START
  |
  v
SELECT SERVER / CLIENT
  |
  v
CREATE SOCKET
  |
  v
BIND / PREPARE
  |
  v
SEND / RECEIVE
  |
  v
MEASURE RTT
  |
  v
DISPLAY RESULT
  |
  v
CLOSE SOCKET
  |
  v
END
```

---

## 10. Implementation Plan

The implementation was carried out in incremental stages.

### Step 1 – Linux Device Driver

The Linux character device driver was implemented using C.

Tasks included:

- Creating the kernel module source
- Registering the character device
- Creating `/dev/chaos_driver`
- Implementing `open()`
- Implementing `read()`
- Implementing `write()`
- Implementing IOCTL operations
- Validating configuration values
- Storing configuration
- Testing the device from user space

### Step 2 – C++ Control Application

The C++ application was implemented using:

```text
src/main.cpp
```

Tasks included:

- User input handling
- Network profile selection
- Configuration creation
- Device opening
- IOCTL communication
- Configuration retrieval
- Status display

### Step 3 – Network Configuration

Linux `tc netem` integration was implemented.

The application configures:

- Latency
- Packet Loss
- Jitter

The selected interface is:

```text
enp0s3
```

The application uses `fork()` and `execl()` to execute `/sbin/tc`.

### Step 4 – Network Profiles

Four predefined profiles were implemented:

- 3G
- 4G
- Wi-Fi
- Satellite

The profile values are:

| Profile | Latency | Jitter | Packet Loss |
|---|---:|---:|---:|
| 3G | 100 ms | 30 ms | 2% |
| 4G | 50 ms | 15 ms | 1% |
| Wi-Fi | 20 ms | 5 ms | 1% |
| Satellite | 600 ms | 50 ms | 2% |

### Step 5 – TCP and UDP Testing

TCP and UDP socket test programs were implemented separately.

TCP:

```text
tests/tcp_test.cpp
```

UDP:

```text
tests/udp_test.cpp
```

Both programs provide client/server communication and round-trip time measurement.

### Step 6 – Integration

The seven project features were integrated:

- Latency / Delay
- Packet Loss
- Jitter
- Network Profiles
- TCP Testing
- UDP Testing
- Linux Device Driver

### Step 7 – Testing and Debugging

Testing included:

- Linux device driver compilation
- Driver loading
- Character device verification
- IOCTL SET testing
- IOCTL GET testing
- Network profile testing
- `tc netem` verification
- TCP socket testing
- UDP socket testing
- CMake build testing
- System integration testing

---

## 11. Development Environment and Build Structure

The project was developed and tested on Ubuntu Linux running inside Oracle VirtualBox.

### Development Environment

Host environment:

```text
Windows 11
Oracle VirtualBox
```

Guest environment:

```text
Ubuntu Linux
```

Kernel version used during development:

```text
7.0.0-38-generic
```

### Development Tools

The project uses:

- GCC
- G++
- Make
- CMake
- Git
- Linux kernel headers
- iproute2
- `tc`

### Programming Languages

The project source code uses:

```text
C
C++
```

The Linux device driver is implemented in C.

The user-space application and testing programs are implemented in C++.

### Project Structure

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

The Linux device driver is compiled using the Linux kernel build system.

The C++ user-space programs can be built using CMake.

The verified CMake targets are:

```text
chaos_emulator
driver_ioctl_test
tcp_test
udp_test
```

---

## 12. Git Branching and Version Control Plan

Git is used to maintain the complete development history of the project.

The project uses the:

```text
master
```

branch.

Development was maintained through incremental commits corresponding to major project milestones.

The actual Git history includes:

```text
Initial project structure and Stage 1 documentation
Stage 2 requirements and development plan
Stage 3 system design and architecture
Implement Linux character device driver
Add initial C++ prototype and network testing
Add Stage 5 testing and integration documentation
Add Stage 6 final implementation and presentation
Ignore compiled test executables
Complete project README and execution instructions
Add CMake build configuration
```

Git is used to track:

- Source code
- Driver implementation
- Test programs
- Documentation
- Build configuration
- README
- Project structure

Compiled executables and CMake build output are excluded using `.gitignore`.

The repository is maintained with a clean working tree after completed development changes.

---

## 13. Documentation and Progress Tracking

The project documentation is divided into six development stages:

```text
docs/Stage1/
docs/Stage2/
docs/Stage3/
docs/Stage4/
docs/Stage5/
docs/Stage6/
```

Each stage documents the corresponding development activity.

### Stage 1

Project Introduction

Documents:

- Project objective
- Problem statement
- Project scope
- Seven project features

### Stage 2

Requirements and Development Plan

Documents:

- Functional requirements
- Technical requirements
- Development planning
- Project scope

### Stage 3

System Design and Architecture

Documents:

- System architecture
- Component architecture
- Data structures
- Architecture diagram
- UML design
- Sequence design
- State machine design
- Implementation plan
- Development environment
- Git plan

### Stage 4

Initial Implementation and Prototype

Documents the initial implementation of:

- Linux device driver
- C++ control application
- Network profiles
- `tc netem`
- TCP testing
- UDP testing

### Stage 5

Testing, Integration and Improvement

Documents:

- Driver testing
- IOCTL testing
- Network profile testing
- TCP testing
- UDP testing
- Integration testing
- Known limitations
- Improvements

### Stage 6

Final Implementation and Presentation

Documents:

- Final architecture
- Final implementation
- Testing summary
- Build and execution
- Git version control
- Limitations
- Presentation plan

The Git history provides additional evidence of incremental development.

---

## 14. Stage 3 Completion Criteria

Stage 3 is complete when the following design elements are documented:

- Overall system architecture
- Component architecture
- System architecture flow
- Data structures
- System architecture diagram
- UML class/component design
- UML sequence design
- UML state machine design
- Implementation plan
- Development environment
- Project directory structure
- Git and version-control plan
- Documentation and progress-tracking plan

The Stage 3 design corresponds to the implemented project structure and provides the technical foundation for the implementation, testing, integration, and final presentation stages.

The project maintains the following seven defined features:

1. Latency / Delay
2. Packet Loss
3. Jitter
4. Network Profiles
5. TCP Testing
6. UDP Testing
7. Linux Device Driver

### Important Testing Limitation

The TCP and UDP functional tests were performed using endpoints within the same Ubuntu virtual machine.

The local test traffic was routed through the Linux loopback interface:

```text
lo
```

rather than:

```text
enp0s3
```

Therefore, the TCP and UDP tests demonstrate successful socket communication and RTT measurement, but their local RTT measurements do not demonstrate the effect of `tc netem` on the `enp0s3` interface.

The `tc netem` configuration itself was successfully applied and verified on `enp0s3`.

A separate network endpoint would be required for complete end-to-end validation of the effect of the network impairment on TCP and UDP traffic.

This limitation is documented to accurately represent the testing environment and results.
