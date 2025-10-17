//
// Producer process for the Producer-Consumer problem
// Author: Chris Kessell
// Date: 10/16/25
//
// This program produces items and places them into a shared buffer using semaphores
// for synchronization. The buffer size is 2, and semaphores ensure mutual exclusion
// and correct signaling between producer and consumer.
//

#include "table.h"
#include <semaphore.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

volatile sig_atomic_t running = 1;

// Handler for Ctrl+C to gracefully exit
void handle_sigint(int sig) {
    running = 0;
}

int main() {
    signal(SIGINT, handle_sigint);  // Register signal handler

    // Create shared memory for buffer
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedTable));
    auto *shared = static_cast<SharedTable *>(mmap(nullptr, sizeof(SharedTable),
                                                    PROT_READ | PROT_WRITE,
                                                    MAP_SHARED, shm_fd, 0));
    shared->count = 0;  // Initialize buffer count

    // Open semaphores
    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0666, BUFFER_SIZE); // Tracks empty slots
    sem_t *full  = sem_open(SEM_FULL, O_CREAT, 0666, 0);            // Tracks filled slots
    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);           // Binary semaphore for mutual exclusion

    int item = 1;

    // Producer loop
    do {
        int sval;
        sem_getvalue(empty, &sval);
        if (sval == 0) {
            std::cout << "[Producer] Waiting: buffer full\n"; // Waiting message
        }

        sem_wait(empty); // Wait for empty slot
        sem_wait(mutex); // Enter critical section

        // Produce the item and add to buffer
        shared->buffer[shared->count++] = item;
        std::cout << "[Producer] Produced: " << item
                  << " | Buffer count: " << shared->count << std::endl;
        item++;

        sem_post(mutex); // Exit critical section
        sem_post(full);  // Signal consumer that buffer has item

        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate production time

    } while (running);

    std::cout << "\n[Producer] Exiting...\n";

    // Clean up resources
    munmap(shared, sizeof(SharedTable));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    return 0;
}