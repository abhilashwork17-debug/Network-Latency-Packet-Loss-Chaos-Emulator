# Stage 5 – Testing, Integration and Improvement

## 1. Testing Overview

Stage 5 focuses on testing the implemented Network Latency and Packet Loss Chaos Emulator and verifying the integration between the C++ application, Linux device driver, and Linux networking subsystem.

The following components were tested:

- Linux character device driver
- C++ driver communication using IOCTL
- Latency / Delay
- Packet Loss
- Jitter
- Network Profiles
- TCP Testing
- UDP Testing
- `tc netem` network configuration

The tests were performed in the Ubuntu Linux virtual machine environment.

---

## 2. Linux Device Driver Testing

The Linux character device driver was tested by loading the kernel module and verifying the device node.

The driver was successfully loaded using:

```bash
sudo insmod driver/chaos_driver.ko
```

The device node was verified as:

```text
/dev/chaos_driver
```

The driver was tested using a C++ IOCTL test program.

The following configuration was sent to the driver:

- Latency: 100 ms
- Packet Loss: 5%
- Jitter: 20 ms

The driver successfully received and stored the configuration.

The same configuration was then retrieved from the driver using the GET configuration IOCTL.

Test result:

```text
Configuration sent to driver:
Latency: 100 ms
Packet Loss: 5%
Jitter: 20 ms

Configuration received from driver:
Latency: 100 ms
Packet Loss: 5%
Jitter: 20 ms
```

This confirms successful communication between the C++ application and the Linux character device driver.

---

## 3. Network Impairment Testing

The network impairment functionality was tested using the Linux `tc netem` subsystem.

The C++ application applies the selected configuration to the `enp0s3` network interface.

The following parameters were tested:

- Latency / Delay
- Jitter
- Packet Loss

For example, a configuration containing:

```text
Latency: 100 ms
Jitter: 30 ms
Packet Loss: 2%
```

was applied through `tc netem`.

The configuration was verified using:

```bash
sudo tc qdisc show dev enp0s3
```

The output confirmed the configured delay, jitter and packet-loss values.

This verifies that the C++ application successfully integrates with the Linux networking subsystem.

---

## 4. Network Profile Testing

All four predefined network profiles were tested.

| Profile | Latency | Jitter | Packet Loss |
|---|---:|---:|---:|
| 3G | 100 ms | 30 ms | 2% |
| 4G | 50 ms | 15 ms | 1% |
| Wi-Fi | 20 ms | 5 ms | 1% |
| Satellite | 600 ms | 50 ms | 2% |

Each profile was selected through the C++ application.

The selected values were:

1. Sent from the C++ application to the Linux device driver.
2. Retrieved from the driver using IOCTL.
3. Applied to the `enp0s3` interface using `tc netem`.
4. Verified using the `tc qdisc show` command.

All four profiles were successfully applied and verified.

The profile values are project-defined simulation parameters used to represent different network conditions.

---

## 5. TCP Testing

A TCP client-server test program was implemented in C++.

The TCP server listens on port `5000` and waits for client connections.

The TCP client connects to the server and sends a test message.

The server receives the message and sends a response back to the client.

The round-trip time is measured by the client.

Example test result:

```text
Connecting to TCP server...
Connected successfully.

TCP response received: TCP test message from Chaos Emulator
Round-trip time: 1 ms
```

The test confirms that the TCP client and server can successfully establish a connection and exchange data.

---

## 6. UDP Testing

A UDP client-server test program was implemented in C++.

The UDP client sends a test packet to the server.

The server receives the packet and sends a response back to the client.

The client measures the round-trip time.

Example test result:

```text
Sending UDP packet...

UDP response received: UDP test message from Chaos Emulator
Round-trip time: 1 ms
```

The test confirms successful UDP packet transmission and response handling.

---

## 7. System Integration Testing

The complete prototype was tested as an integrated system.

The complete execution flow is:

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
C++ Network Controller
  |
  v
tc netem
  |
  v
enp0s3 Network Interface
```

The integration test confirmed that:

- The C++ application starts successfully.
- A network profile can be selected.
- The configuration is sent to the Linux device driver.
- The configuration can be retrieved from the driver.
- The selected network parameters are applied using `tc netem`.
- The applied configuration can be verified using Linux networking commands.
- TCP and UDP socket tests operate successfully.

---

## 8. Testing Limitation

The TCP and UDP functional tests were performed using endpoints within the same Ubuntu virtual machine.

The tested local destination was routed through the Linux loopback interface (`lo`) rather than the `enp0s3` interface.

Therefore, the TCP and UDP round-trip measurements demonstrate successful socket communication, but they do not by themselves prove the effect of the `tc netem` configuration on those local test packets.

A separate network endpoint would be required to perform a complete end-to-end measurement of the impairment effect on TCP and UDP traffic through `enp0s3`.

This limitation is documented to maintain accurate and reproducible test results.

---

## 9. Improvements Made

The following improvements were made during Stage 5:

- Corrected TCP client/server argument handling.
- Updated the TCP server to continue accepting client connections.
- Added configuration validation in the Linux device driver.
- Added error handling for driver communication.
- Added verification of applied `tc netem` configurations.
- Tested all four network profiles.
- Verified TCP and UDP communication separately.
- Documented the local testing limitation.

---

## 10. Stage 5 Status

Stage 5 successfully verifies the integration of the major project components.

The Linux device driver, C++ application, network profile configuration, `tc netem` integration, TCP testing and UDP testing were tested individually and as part of the complete prototype.

The results provide the foundation for the final implementation, documentation and presentation in Stage 6.
