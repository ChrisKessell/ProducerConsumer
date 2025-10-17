//
// Consumer process for the Producer-Consumer problem
// Author: Chris Kessell
// Date: 10/16/25
//
// This program consumes items from a shared buffer using semaphores for
// synchronization. It waits when the buffer is empty and signals the producer
// when a slot becomes free.
//

#include "table.h"
#include <semaphore.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) {
    running = 0;
}

int main() {
    signal(SIGINT, handle_sigint);

    // Access shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    auto *shared = static_cast<SharedTable *>(mmap(nullptr, sizeof(SharedTable),
                                                    PROT_READ | PROT_WRITE,
                                                    MAP_SHARED, shm_fd, 0));

    // Open semaphores
    sem_t *empty = sem_open(SEM_EMPTY, 0);
    sem_t *full  = sem_open(SEM_FULL, 0);
    sem_t *mutex = sem_open(SEM_MUTEX, 0);

    // Consumer loop
    do {
        int sval;
        sem_getvalue(full, &sval);
        if (sval == 0) {
            std::cout << "[Consumer] Waiting: buffer empty\n"; // Waiting message
        }

        sem_wait(full);  // Wait for item in buffer
        sem_wait(mutex); // Enter critical section

        if (shared->count > 0) {
            int item = shared->buffer[--shared->count]; // Consume item
            std::cout << "[Consumer] Consumed: " << item
                      << " | Buffer count: " << shared->count << std::endl;
        }

        sem_post(mutex); // Exit critical section
        sem_post(empty); // Signal producer that slot is free

        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Simulate consumption time

    } while (running);

    std::cout << "\n[Consumer] Exiting...\n";

    // Clean up
    munmap(shared, sizeof(SharedTable));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    return 0;
}