# Codexion

*This project has been created as part of the 42 curriculum by nabil4416.*

## Description

**Codexion** is a concurrent programming project that simulates coders competing for limited USB dongles in a shared workspace. It implements the classic *Dining Philosophers* problem using POSIX threads, mutexes, and synchronization primitives.

### Project Goal
- Simulate multiple coders working in parallel threads
- Manage shared resources (dongles) safely using mutexes
- Implement fair scheduling strategies (FIFO and EDF)
- Prevent race conditions, deadlocks, and starvation
- Detect burnout and stop cleanly

## Instructions

### Build
```bash
make