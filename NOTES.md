# Codexion Notes

## Project goal
Simulate coders competing for shared dongles using threads and synchronization.

## Core concepts
- Thread: independent execution flow
- Mutex: protects shared resources
- Race condition: multiple threads access shared data unsafely
- Deadlock: threads wait forever for locked resources
- Scheduling:
  - FIFO = first in, first out
  - EDF = earliest deadline first

## Project architecture
- main.c: parse args, init data, launch simulation
- init.c: initialize structs, mutexes, threads
- simulation.c: main thread logic
- monitor.c: detect death / stop condition
- cleanup.c: destroy mutexes, free memory

## What I understand
- Each coder runs in its own thread
- Dongles are shared resources protected by mutexes
- A monitor thread or loop checks if a coder dies
- Timing must be synchronized carefully

## What I do not fully understand yet
- Exact moment to lock/unlock dongles
- How to avoid deadlock safely
- Difference between scheduler logic and normal philosopher logic
- How to stop all threads cleanly

## Bugs encountered
- Possible deadlock when all coders take one dongle first
- Desynchronized timestamps
- Threads not stopping cleanly
- Race condition on shared stop flag

## Questions for Copilot
- Explain one safe dongle acquisition strategy
- Compare odd/even strategy vs waiter strategy
- Explain how to protect shared stop state
