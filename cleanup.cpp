//
// Cleanup shared memory and semaphores
// Author: Chris Kessell
// Date: 10/16/25
//
// This program removes shared memory and semaphores used by producer-consumer.
//

#include "table.h"
#include <iostream>

int main() {
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);

    std::cout << "Cleaned up shared memory and semaphores.\n";
    return 0;
}