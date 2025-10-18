# Producer-Consumer Problem in C++ with Shared Memory and Semaphores

## Table of Contents
- [Description](#description)
- [Files](#files)
- [Requirements](#requirements)
- [Compilation](#compilation)
- [Usage](#usage)
- [Example](#example)
- [Explanation](#explanation)

## Description
This project demonstrates the classic Producer-Consumer problem in C++ using POSIX shared memory and semaphores for process synchronization. The producer adds items to a shared buffer, while the consumer removes them. Synchronization ensures that the producer waits when the buffer is full and the consumer waits when the buffer is empty.

## Files
- `producer.cpp` – Implements the producer process.  
- `consumer.cpp` – Implements the consumer process.  
- `cleanup.cpp` – Cleans up shared memory and semaphores.  
- `table.h` – Contains shared memory structure and semaphore definitions.

## Requirements
- Linux environment  
- C++ compiler (`g++`) with POSIX support  
- POSIX shared memory and semaphores (`-lrt` and `-pthread` flags)

## Compilation
Compile each source file separately using the following commands:

```bash
g++ producer.cpp -pthread -lrt -o producer
g++ consumer.cpp -pthread -lrt -o consumer
g++ cleanup.cpp -pthread -lrt -o cleanup
```

## Usage
1. Start the producer and consumer processes in separate terminals or in the background:

```bash
./producer & ./consumer &
```

2. Run cleanup after stopping the producer and consumer to remove shared memory and semaphores:

```bash
./cleanup
```

3. Stop processes with Ctrl+C.

## Example
Sample console output when running the producer and consumer:

<img width="899" height="683" alt="image" src="https://github.com/user-attachments/assets/a8642361-3d84-49ff-a7d3-c155745c2741" />

## Explanation

In this run, only the `[Producer] Waiting: buffer full` message appears. This occurs because:

- The producer adds items to the buffer faster than the consumer removes them.
- The buffer reaches its maximum size (`BUFFER_SIZE = 2`) quickly, triggering the "buffer full" message.
- The consumer never attempts to consume from an empty buffer during this run, so `[Consumer] Waiting: buffer empty` does not appear.

Due to the concurrent nature of the producer and consumer processes, the output may vary in different runs:

- If the consumer runs faster than the producer at any moment, the buffer may become empty, triggering `[Consumer] Waiting: buffer empty`.
- Sleep intervals and OS scheduling affect which waiting messages appear.
- This demonstrates real-world concurrency where timing impacts the program output.

