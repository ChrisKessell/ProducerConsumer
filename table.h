//
// Shared buffer table for Producer-Consumer problem
// Author: Chris Kessell
// Date: 10/16/25
//

#ifndef PRODUCERCONSUMER_TABLE_H
#define PRODUCERCONSUMER_TABLE_H

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define SHM_NAME "/my_shared_buffer"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_MUTEX "/sem_mutex"
#define BUFFER_SIZE 2

struct SharedTable {
    int buffer[BUFFER_SIZE]; // Shared buffer
    int count;               // Number of items currently in buffer
};

#endif //PRODUCERCONSUMER_TABLE_H