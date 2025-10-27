# Producer-Consumer Problem in C++ with Shared Memory and Semaphores

## Table of Contents
- [Description](#description)
- [Files](#files)
- [Requirements](#requirements)
- [How It Works](#how-it-works)
    - [Synchronization Mechanisms](#synchronization-mechanisms)
    - [Shared Memory Structure](#shared-memory-structure)
    - [Process Flow](#process-flow)
- [Compilation](#compilation)
- [Usage](#usage)
- [Examples](#examples)
    - [Example #1](#example-1)
    - [Example #2](#example-2)

## Description
This project demonstrates the classic Producer-Consumer problem in C++ using POSIX shared memory and semaphores for process synchronization. The producer adds items to a shared buffer, while the consumer removes them. Synchronization ensures that the producer waits when the buffer is full and the consumer waits when the buffer is empty.

***NOTE***

I spoke with Professor Jinhui Ma and he told me it was okay to centralize all my Documentation (program description & explanation of key components), Readme (List and briefly describe each file included in your repository & usage instructions) into a single ReadMe file.

<img width="1039" height="152" alt="image" src="https://github.com/user-attachments/assets/e2f80453-4c54-4ace-9109-baadd27da657" />


## Files
- `producer.cpp` – Implements the producer process.  
- `consumer.cpp` – Implements the consumer process.  
- `cleanup.cpp` – Cleans up shared memory and semaphores.  
- `table.h` – Contains shared memory structure and semaphore definitions.

## Requirements
- Linux environment  
- C++ compiler (`g++`) with POSIX support  
- POSIX shared memory and semaphores (`-lrt` and `-pthread` flags)

## How It Works

### Synchronization Mechanisms
The program uses **three semaphores** to coordinate access to the shared buffer:

1. **`empty`** (initialized to `BUFFER_SIZE`): Counts available empty slots in the buffer
    - Producer waits on this semaphore before adding an item
    - Consumer signals this semaphore after removing an item

2. **`full`** (initialized to `0`): Counts filled slots in the buffer
    - Consumer waits on this semaphore before removing an item
    - Producer signals this semaphore after adding an item

3. **`mutex`** (initialized to `1`): Binary semaphore for mutual exclusion
    - Ensures only one process accesses the shared buffer at a time
    - Prevents race conditions and data corruption

### Shared Memory Structure
The shared memory segment contains a `SharedTable` structure defined in `table.h`:

### Process Flow

**Producer Process:**
1. Check if buffer is full (empty semaphore = 0)
2. Wait for an empty slot (`sem_wait(empty)`)
3. Acquire mutex lock (`sem_wait(mutex)`)
4. Add item to buffer and increment count
5. Release mutex lock (`sem_post(mutex)`)
6. Signal that buffer has a new item (`sem_post(full)`)
7. Sleep to simulate production time
8. Repeat until interrupted

**Consumer Process:**
1. Check if buffer is empty (full semaphore = 0)
2. Wait for a filled slot (`sem_wait(full)`)
3. Acquire mutex lock (`sem_wait(mutex)`)
4. Remove item from buffer and decrement count
5. Release mutex lock (`sem_post(mutex)`)
6. Signal that buffer has an empty slot (`sem_post(empty)`)
7. Sleep to simulate consumption time
8. Repeat until interrupted

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

## Examples

### Example #1
Sample console output when running the producer(1000ms) and consumer(1500ms):

<img width="899" height="683" alt="image" src="https://github.com/user-attachments/assets/a8642361-3d84-49ff-a7d3-c155745c2741" />

### Explanation

In this run, only the `[Producer] Waiting: buffer full` message appears. This occurs because:

- The producer adds items to the buffer faster than the consumer removes them.
- The buffer reaches its maximum size (`BUFFER_SIZE = 2`) quickly, triggering the "buffer full" message.
- The consumer never attempts to consume from an empty buffer during this run, so `[Consumer] Waiting: buffer empty` does not appear.

### Example #2
Sample console output when running the producer(1500ms) and consumer(1000ms):

<img width="899" height="850" alt="image" src="https://github.com/user-attachments/assets/ee828414-6534-407d-a155-81e1934e4f6c" />


### Explanation

Due to the concurrent nature of the producer and consumer processes, the output may vary in different runs:

- If the consumer runs faster than the producer at any moment, the buffer may become empty, triggering `[Consumer] Waiting: buffer empty`.
- Sleep intervals and OS scheduling affect which waiting messages appear.
- This demonstrates real-world concurrency where timing impacts the program output.
