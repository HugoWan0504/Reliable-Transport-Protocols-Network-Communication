# Reliable Transport Protocols & Network Communication

## Overview

This project implements a series of **client–server networking protocols** that progressively build from **basic UDP communication** to **reliable data transfer mechanisms**. The work explores how transport-layer reliability, ordering, and fault tolerance can be implemented **at the application level**, mirroring core ideas behind TCP.

The project emphasizes **socket programming, protocol design, timeout handling, and correctness under packet loss**.

---

## Project Structure & Components

### L1 — Basic UDP Client–Server Message Exchange

Implements a minimal **UDP-based client–server interaction**, establishing foundational concepts such as socket creation, port binding, and message exchange.

**Key concepts:**

* UDP sockets
* Send/receive semantics
* Stateless communication

---

### L2 — Reliable Message Delivery over UDP

Extends basic UDP communication with **acknowledgment-based reliability**, ensuring messages are delivered correctly despite UDP’s unreliability.

**Key concepts:**

* Explicit ACK messages
* Sequence tracking
* Application-level reliability

---

### L3 — Transport Protocol Comparison: TCP vs UDP

Implements both **TCP and UDP client–server pairs** to contrast transport-layer semantics and behavior.

**Key concepts:**

* Connection-oriented vs connectionless protocols
* TCP stream behavior
* UDP datagram behavior

---

### L4 — Stop-and-Wait ARQ Protocol

Implements the **Stop-and-Wait Automatic Repeat reQuest (ARQ)** protocol to ensure reliable delivery over an unreliable channel.

**Key concepts:**

* Timeouts and retransmission
* ACK-based flow control
* One-packet sliding window

---

### L5 — Sliding Window Reliability with Timeout-Based Retransmission

Extends reliability mechanisms by allowing repeated retransmissions and simulating more realistic network failures.

**Key concepts:**

* Persistent retransmission under packet loss
* Robust timeout handling
* Foundations of sliding window protocols

---

## Core Networking Concepts Demonstrated

* UDP and TCP socket programming
* Transport-layer protocol design
* Reliable data transfer over unreliable networks
* Timeout and retransmission strategies
* Client–server synchronization and correctness

---

## Implementation Details

* Language: **C**
* APIs: **POSIX sockets**
* Design emphasizes:

  * Clear protocol logic
  * Deterministic behavior
  * Explicit error handling

Each protocol variant is implemented as a **client–server pair**, with logging output to trace protocol behavior.

---

## Example Outputs

The repository includes execution logs demonstrating:

* Successful packet exchange
* Timeout detection
* Retransmission behavior
* Completion under packet loss

---

## Project Status

* All protocol variants implemented
* Tested under normal and loss-induced scenarios
* No known functional issues

---

## Author

**Hugo Wan**

---

## Notes

This project demonstrates how **transport-layer reliability can be constructed incrementally**, providing insight into the design principles behind real-world protocols such as TCP.
