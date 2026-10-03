# Stage 2 – Requirements & Development Plan

## 1. Purpose of the Stage

Stage 2 defines the functional requirements, technical requirements, development approach, and implementation plan for the Network Latency and Packet Loss Chaos Emulator.

The purpose of this stage is to clearly define what the system must provide before implementation begins.

The project is limited to seven core features:

1. Latency / Delay
2. Packet Loss
3. Jitter
4. Network Profiles
5. TCP Testing
6. UDP Testing
7. Linux Device Driver

---

## 2. Functional Requirements

### FR-01: Latency / Delay

The system shall allow the user to configure network latency.

The configured delay shall be applied to the selected network interface during testing.

The system shall allow the delay configuration to be changed between experiments.

---

### FR-02: Packet Loss

The system shall allow the user to configure packet loss as a percentage.

The configured packet-loss condition shall be applied during network testing.

The system shall support changing the packet-loss value for different experiments.

---

### FR-03: Jitter

The system shall allow the user to configure jitter.

Jitter shall represent variation in packet delay rather than a fixed additional delay.

The configured jitter shall be applied together with the selected network conditions.

---

### FR-04: Network Profiles

The system shall provide four predefined network profiles:

- 3G
- 4G
- Wi-Fi
- Satellite

Each profile shall contain project-defined values for:

- Latency
- Packet Loss
- Jitter

The profiles are intended for controlled experimentation and do not represent exact measurements of every real-world network.

---

### FR-05: TCP Testing

The system shall provide a basic TCP client/server testing mechanism.

The test shall establish TCP communication between the required endpoints.

TCP testing shall be performed under the selected network conditions.

---

### FR-06: UDP Testing

The system shall provide a basic UDP client/server testing mechanism.

The test shall transmit UDP data between the required endpoints.

UDP testing shall be performed under the selected network conditions.

---

### FR-07: Linux Device Driver

The system shall contain a Linux character device driver implemented as a kernel module.

The driver shall provide a device interface through `/dev/`.

The user-space C++ application shall communicate with the driver using Linux system-programming interfaces such as:

- `open()`
- `read()`
- `write()`
- `ioctl()`

The driver shall receive and maintain the required network configuration information.

---

## 3. Non-Functional Requirements

### 3.1 Operating System

The project shall run on Linux.

The development and testing environment shall use Ubuntu Linux.

### 3.2 Programming Languages

The project implementation shall use:

- C for the Linux device driver
- C++ for the user-space application

Python, Java, or other programming languages shall not be used for the actual project implementation.

### 3.3 Performance

The application should respond quickly to configuration changes and testing commands.

The system should avoid unnecessary processing during network testing.

### 3.4 Reliability

Invalid configuration values should be detected before they are applied.

The system should provide clear error messages when a requested operation fails.

### 3.5 Maintainability

The project shall be organized into separate components for:

- Device driver
- C++ application
- Network control
- TCP testing
- UDP testing
- Header files
- Tests
- Documentation

### 3.6 Version Control

Git shall be used throughout development.

Major development stages and functional milestones shall be committed separately.

---

## 4. Technical Requirements

The development environment requires:

| Component | Requirement |
|---|---|
| Operating System | Ubuntu Linux |
| Application Language | C++ |
| Driver Language | C |
| Compiler | GCC / G++ |
| Build System | CMake |
| Version Control | Git |
| Network Control | Linux `tc` / `netem` |
| Network Protocols | TCP and UDP |
| Kernel Interface | Linux Character Device |
| Kernel Headers | Installed for running kernel |

---

## 5. System Requirements

The system requires a Linux machine or virtual machine with:

- At least 4 GB RAM
- Multi-core CPU
- Linux kernel with required networking support
- Root privileges for network configuration
- Network interface available for testing
- Linux kernel headers for driver compilation

The project is being developed and tested inside an Ubuntu virtual machine.

---

## 6. Input Requirements

The system will accept network-condition parameters such as:

```text
Latency
Packet Loss
Jitter

The input will also include the selection of a predefined network profile:

- 3G
- 4G
- Wi-Fi
- Satellite

The user will also select the type of transport test:

- TCP
- UDP
---

## 7. Output Requirements

The system should provide clear information about:

- Selected network profile
- Configured latency
- Configured packet loss
- Configured jitter
- Driver communication status
- TCP test status
- UDP test status
- Errors encountered during configuration or testing
---

## 8. Proposed Development Modules

### Module 1 – Linux Device Driver

Responsible for:

- Character device creation
- Device registration
- Configuration handling
- User-space communication
- `ioctl` interface

### Module 2 – C++ Control Application

Responsible for:

- Accepting user configuration
- Selecting network profiles
- Communicating with the device driver
- Starting network tests

### Module 3 – Network Control

Responsible for applying:

- Latency
- Packet Loss
- Jitter

using Linux networking facilities.

### Module 4 – TCP Testing

Responsible for establishing and testing TCP communication under configured network conditions.

### Module 5 – UDP Testing

Responsible for sending and receiving UDP data under configured network conditions.
---

## 9. Development Plan

The project will be developed incrementally.

### Phase 1 – Project Setup

Tasks:

- Create project repository
- Configure Git
- Create directory structure
- Configure CMake
- Prepare documentation

### Phase 2 – Linux Device Driver

Tasks:

- Create kernel module
- Implement character device
- Define driver interface
- Implement configuration handling
- Test user-space communication

### Phase 3 – C++ Application

Tasks:

- Create C++ application structure
- Implement configuration handling
- Implement driver communication
- Add input validation

### Phase 4 – Network Conditions

Tasks:

- Implement latency configuration
- Implement packet-loss configuration
- Implement jitter configuration
- Connect the configuration with Linux network control

### Phase 5 – Network Profiles

Tasks:

- Define 3G profile
- Define 4G profile
- Define Wi-Fi profile
- Define Satellite profile
- Test profile selection

### Phase 6 – TCP and UDP Testing

Tasks:

- Implement TCP client/server testing
- Implement UDP client/server testing
- Test communication under different network conditions

### Phase 7 – Integration and Testing

Tasks:

- Integrate the driver and C++ application
- Test all seven project features
- Fix implementation errors
- Verify different network configurations

### Phase 8 – Documentation and Demonstration

Tasks:

- Complete project documentation
- Record implementation evidence
- Update README
- Prepare GitHub repository
- Prepare final demonstration
---

## 10. Development Environment

The project is being developed inside an Ubuntu Linux virtual machine running on Oracle VirtualBox.

The development environment contains:

- GCC/G++
- CMake
- Git
- Linux kernel headers
- `iproute2`
- `tc`
- `iperf3` for optional network verification

The main project implementation will remain in C and C++.

---

## 11. Git Development Strategy

Git will be used to maintain the development history.

The project will follow this general workflow:

Create / Modify Code
        ↓
Compile
        ↓
Test
        ↓
Fix Errors
        ↓
Git Add
        ↓
Git Commit

Major milestones will receive separate commits, such as:

- Initial project structure
- Stage 1 documentation
- Stage 2 requirements
- Stage 3 system design
- Linux driver implementation
- C++ application implementation
- Latency implementation
- Packet loss implementation
- Jitter implementation
- Network profiles
- TCP testing
- UDP testing
- Integration testing
- Final documentation
---

## 12. Testing Strategy

Testing will be performed incrementally.

### Driver Testing

The character device will be checked for:

- Successful module loading
- Device creation
- User-space communication
- Valid configuration handling

### Network Condition Testing

Each network condition will be tested separately:

- Latency
- Packet Loss
- Jitter

### Profile Testing

Each predefined profile will be tested:

- 3G
- 4G
- Wi-Fi
- Satellite

### Transport Testing

Both transport protocols will be tested:

- TCP
- UDP

### Integration Testing

The complete flow will be tested:

C++ Application
      ↓
Device Driver
      ↓
Network Configuration
      ↓
TCP / UDP Test
---

## 13. Risk and Mitigation

| Risk | Mitigation |
|---|---|
| Driver compilation failure | Use matching Linux kernel headers |
| Kernel API changes | Develop against the running Ubuntu kernel |
| Permission errors | Use appropriate root privileges for driver/network operations |
| Incorrect network configuration | Validate parameters before applying them |
| TCP/UDP test failure | Test networking modules independently before integration |
| Virtual machine networking issues | Verify the Linux network interface before testing |

---

## 14. Expected Development Outcome

At the end of the development process, the project is expected to provide a working Linux-based network testing system containing the seven defined features.

The final system will combine:

Linux Device Driver
        +
C++ Control Application
        +
Latency
        +
Packet Loss
        +
Jitter
        +
Network Profiles
        +
TCP Testing
        +
UDP Testing

The completed system will be tested in a controlled Ubuntu environment and documented through the project's Git repository.
